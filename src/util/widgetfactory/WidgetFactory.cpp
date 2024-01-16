//
// Created by stupid_coder_jyy on 2024/1/14.
//
#include "WidgetFactory.h"
#include "MemUtil.h"
#include "NBT.h"
#include "Preconditions.h"
#include "WidgetFactoryStorage.h"
#include "QssParser.h"
#include "FactoryInit.h"

#include <QLayout>
#include <utility>

WidgetFactory::WidgetFactory(QString id):
        id(std::move(id)),
        customSuppliers(new QMap<QString, Supplier>()),
        customEmptyInstances(new QMap<QString, StandardWidget*>()),
        parentFactory(nullptr){
}

WidgetFactory::WidgetFactory(WidgetFactory *parent, QString id, NBT *nbt):
        id(std::move(id)),
        customSuppliers(parent->customSuppliers),
        customEmptyInstances(parent->customEmptyInstances),
        parentFactory(parent){
    parent->childFactories.insert(id, this);
    source = nbt;
}

void WidgetFactory::setSource(NBT *nbt) {
    Preconditions::checkState(state == Empty, "WidgetFactory::setSource", "required state:Empty");
    Preconditions::checkArgument(nbt, "WidgetFactory::setSource", "null nbt");
    source = nbt;
}

QWidget* WidgetFactory::apply(QWidget *parent, QWidget *target) {
    if (state != Ready) {
        throwInFunc("factory '" + id + "' is not in ready state");
    }
    workingWidget = target;
    try {
        for (auto& handler : handlers) {
            handler(workingWidget);
        }
        if (parent) {
            workingWidget->setParent(parent);
        }
    } catch (Error& e) {
        PrintErrorHandler().onErrorCaught(e);
    }
    return workingWidget;
}

void WidgetFactory::parse() {
    if (state != Empty) {
        throwInFunc("factory '" + id + "' is not in empty state");
    }
    state = Parsing;
    try {
        auto* stdWidget = parseWidgetType(source);
        stdWidget->preParsing(handlers, source);
        parseQss(source);
        stdWidget->postParsing(handlers, source);
        parseChildren(source);
        stdWidget->finishedParsing(handlers, source);
        state = Ready;
    } catch (Error& e) {
        e.why = "failed to parse factory '" + id + "': " + e.why;
        PrintErrorHandler().onErrorCaught(e);
        state = Broken;
    }
}

StandardWidget* WidgetFactory::parseWidgetType(NBT *nbt) {
    //外部加载器
    WidgetFactory* loader = nullptr;
    if (nbt->contains("loader", Data::STRING)) {
        Identifier loc = nbt->get("loader")->asString()->getLoc();
        loader = WidgetFactoryStorage::getInstance()->get(loc);
        if (!loader) {
            throwInFunc("factory not found '" + loc.toString() + "'");
        }
    }
    //组件类型，如果使用外部加载器，则stdType会被忽略（别手贱写个不存在的type）
    QString type;
    if (nbt->contains("type", Data::STRING)) {
        type = nbt->get("type")->asString()->get();
    } else {
        type = "std_widget";
    }
    //检查组件类型
    StandardWidget* stdWidget = stdEmptyInstances.value(type);
    if (!stdWidget) {
        stdWidget = customEmptyInstances->value(type);
    }
    if (!stdWidget) {
        throwInFunc("unknown type '" + type + "' in widget '" + id + "'");
    }
    handlers << [this, type, loader](QWidget*) {
        //创建组件对象，设置workingWidget
        if (workingWidget) {
            if (loader) {
                loader->apply(nullptr, workingWidget);
            }
        } else if (loader) {
            workingWidget = loader->apply();
        } else {
            workingWidget = createWidget(type);
        }
        //设置组件名称
        workingWidget->setObjectName(id);
        //设置父组件并将子组件加入父组件的布局中，此时父加载器正在进行调用子加载器的阶段
        if (!parentFactory) {
            return;
        }
        QWidget* parent = parentFactory->workingWidget;
        workingWidget->setParent(parent);
        QLayout* layout = parent->layout();
        if (!layout) {
            return;
        }
        //TODO 特殊布局的逻辑别忘了加上
        layout->addWidget(workingWidget);
    };
    return stdWidget;
}

void WidgetFactory::parseChildren(NBT *nbt) {
    if (!nbt->contains("children", Data::ARR)) {
        return;
    }
    auto* arr = nbt->get("children")->asArray()->get();
    QVector<WidgetFactory*> factories;
    for (auto*& child : *arr) {
        if (child->type == Data::STRING) {
            auto childId = child->asString()->get();
            auto* factory = findFactory(nbt, childId);
            if (!factory) {
                throwInFunc("failed to find child loader '" + childId + "'");
            }
            switch (factory->state) {
                case Broken:
                    throwInFunc("child loader '" + childId + "' is broken");
                case Parsing:
                    throwInFunc("child loader '" + childId + "' is still in parsing");
                case Empty:
                    factory->parse();
                    break;
                default:
                    break;
            }
            factories << factory;
        }
    }
    handlers << [this, factories](QWidget*) {
        for (auto& loader : factories) {
            loader->apply(workingWidget, nullptr);
        }
    };
}

QRegExp WidgetFactory::regexId = QRegExp("[A-Za-z]\\w*");

WidgetFactory *WidgetFactory::findFactory(NBT* nbt, const QString &path) {
    if (path.length() == 0) {
        return nullptr;
    }
    if (path[0] == '@') {
        //从WidgetFactoryStorage找
        int i = path.indexOf('$');
        QString loaderPath = path.mid(1, i > 0 ? i - 1 : path.length() - 1);
        Identifier loaderLoc = Identifier(loaderPath);
        auto* loader = WidgetFactoryStorage::getInstance()->get(loaderLoc);
        if (loader && i > 0) {
            QStringList children = path.mid(i, path.length() - i).split('/');
            for (auto& childId : children) {
                loader = loader->childFactories.value(childId);
                if (loader == nullptr) {
                    return nullptr;
                }
            }
        }
        return loader;
    }
    //从自己的children找
    QString childId = path.mid(1, path.length() - 1);
    if (childFactories.contains(childId)) {
        return childFactories.value(childId);
    }
    if (!regexId.exactMatch(childId)) {
        return nullptr;
    }
    if (!nbt->contains(childId, Data::COMPOUND)) {
        return nullptr;
    }
    return new WidgetFactory(this, childId, nbt->get(childId)->asCompound());
}

void WidgetFactory::parseQss(NBT *nbt) {
    if (!nbt->contains("qss", Data::STRING)) {
        return;
    }
    QString qss = QssParser::translate(nbt->get("qss")->asString()->get());
    handlers << [qss](QWidget* _widget) {
        _widget->setStyleSheet(qss);
    };
}

WidgetFactory::~WidgetFactory() {
    DELETE_MAP(childFactories)
    if (parentFactory == nullptr) {
        DELETE_MAP_P(customEmptyInstances)
        delete customEmptyInstances;
        delete customSuppliers;
        delete source;
    }
}

QMap<QString, WidgetFactory::Supplier> WidgetFactory::stdSuppliers{};
QMap<QString, StandardWidget*> WidgetFactory::stdEmptyInstances{};

QWidget *WidgetFactory::createWidget(const QString &type, QWidget *parent) {
    if (stdSuppliers.contains(type)) {
        return stdSuppliers.value(type)(parent);
    }
    if (customSuppliers->contains(type)) {
        return customSuppliers->value(type)(parent);
    }
    return nullptr;
}

void WidgetFactory::init() {
    FactoryInit::init(&stdSuppliers, &stdEmptyInstances);
    QssParser::init();
}

WidgetFactory *WidgetFactory::fromResource(const Identifier &loc) {
    return WidgetFactoryStorage::getInstance()->get(loc);
}

WidgetFactory *WidgetFactory::fromNbt(const QString& id, NBT *nbt) {
    auto* loader = new WidgetFactory(id);
    loader->setSource(nbt);
    return loader;
}
