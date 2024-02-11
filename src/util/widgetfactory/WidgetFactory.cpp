//
// Created by stupid_coder_jyy on 2024/1/14.
//
#include "WidgetFactory.h"
#include "MemUtil.h"
#include "NBT.h"
#include "WidgetFactoryStorage.h"
#include "QssParser.h"
#include "FactoryInit.h"
#include "WidgetFactoryParsers.h"

#include <QLayout>
#include <utility>

WidgetFactory::WidgetFactory(QString id):
        id(std::move(id)),
        customSuppliers(new QMap<QString, Supplier>()),
        customEmptyInstances(new QMap<QString, StandardWidget*>()),
        parentFactory(nullptr){
}

WidgetFactory::WidgetFactory(WidgetFactory *parent, const QString& id, NBT *nbt):
        id(id),
        customSuppliers(parent->customSuppliers),
        customEmptyInstances(parent->customEmptyInstances),
        parentFactory(parent){
    parent->childFactories.insert(id, this);
    source = nbt;
}

void WidgetFactory::setSource(NBT *nbt) {
    if (state != Empty) {
        throwInFunc("required state:Empty");
    }
    if (!nbt) {
        throwInFunc("null nbt");
    }
    source = nbt;
}

QWidget* WidgetFactory::apply(QWidget *parent, QWidget *target) noexcept{
    try {
        if (state != Ready) {
            throwInFunc("factory '" + id + "' is not in ready state");
        }
        workingWidget = target;
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

void WidgetFactory::parse() noexcept{
    try {
        if (state != Empty) {
            if (state == Ready) {
                return;
            }
            throwInFunc("factory '" + id + "' is not in empty state");
        }
        state = Parsing;
        stdType = parseWidgetType(source);
        stdType->onPreParsing(handlers, source);
        parseQss(handlers,source);
        parseLayout(source);
        parseSize(handlers, source);
        parseStates(source);
        WidgetFactoryParsers::parseSizePolicy(handlers, source);
        parseMargins(source);
        stdType->onPostParsing(handlers, source);
        parseChildren(source);
        parsePointer(source);
        handlers << [](QWidget* target){
            auto* stdWidget = dynamic_cast<StandardWidget*>(target);
            if (stdWidget) {
                stdWidget->setState(0); //设置默认状态
            }
        };
        stdType->onFinishedParsing(handlers, source);
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
    QString type = nbt->getString("type", "std_widget");
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
        layout->addWidget(workingWidget);
    };
    return stdWidget;
}

void WidgetFactory::parseChildren(NBT *nbt) {
    if (!nbt->contains("children", Data::ARR)) {
        return;
    }
    auto arr = nbt->get("children")->asArray()->get();
    QVector<WidgetFactory*> factories;
    for (auto*& child : arr) {
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
    if (childFactories.contains(path)) {
        return childFactories.value(path);
    }
    if (!regexId.exactMatch(path)) {
        return nullptr;
    }
    if (!nbt->contains(path, Data::COMPOUND)) {
        return nullptr;
    }
    return new WidgetFactory(this, path, nbt->get(path)->asCompound());
}

void WidgetFactory::parseQss(Handlers& op, NBT *nbt) {
    if (!nbt->contains("qss", Data::STRING)) {
        return;
    }
    QString qss = QssParser::translate(nbt->getString("qss"));
    op << [qss](QWidget* target) {
        target->setStyleSheet(qss);
    };
}

void WidgetFactory::parseLayout(NBT *nbt) {
    if (!nbt->contains("layout", Data::COMPOUND)) {
        return;
    }
    nbt = nbt->get("layout")->asCompound();
    if (!nbt->contains("type", Data::STRING)) {
        throwInFunc("missing layout type");
    }
    QString type = nbt->get("type")->asString()->get();
    int layoutType;
    if (type == "Horizontal") {
        layoutType = 0;
    } else if (type == "Vertical") {
        layoutType = 1;
    } else {
        throwInFunc("invalid layout type '" + type + "'");
    }
    Qt::Alignment alignment{};
    if (nbt->contains("align", Data::STRING)) {
        alignment = WidgetFactoryParsers::parseAlign(nbt->get("align")->asString()->get());
    }
    QMargins margins{};
    if (nbt->contains("margins", Data::ARR)) {
        margins = WidgetFactoryParsers::parseMargins(nbt->get("margins")->asArray());
    }
    int spacing = 5;
    if (nbt->contains("spacing", Data::INT)) {
        spacing = nbt->get("spacing")->asInt()->get();
    }

    handlers << [=](QWidget* target) {
        QLayout* layout = nullptr;
        switch (layoutType) {
            case 0:
                layout = new QHBoxLayout(target);
                break;
            case 1:
                layout = new QVBoxLayout(target);
                break;
        }
        layout->setAlignment(alignment);
        layout->setContentsMargins(margins);
        layout->setSpacing(spacing);
        target->setLayout(layout);
    };
}

void WidgetFactory::parseMargins(NBT *nbt) {
    if (!nbt->contains("margins", Data::ARR)) {
        return;
    }
    QMargins margins = WidgetFactoryParsers::parseMargins(nbt->get("margins")->asArray());
    handlers << [margins](QWidget* target) {
        target->setContentsMargins(margins);
    };
}

void WidgetFactory::parseStates(NBT *nbt) {
    if (!nbt->contains("states", Data::COMPOUND)) {
        return;
    }
    auto& children = nbt->get("states")->asCompound()->get();
    for (auto it = children.begin() ; it != children.end() ; it++) {
        if (it.value()->type != Data::COMPOUND) {
            continue;
        }
        QString stateKey = it.key();
        NBT* stateTag = it.value()->asCompound();
        if (stateKey == "global") {
            parseSingleState(globalResponders, stateTag);
        } else if (stateKey[0] == 's'){
            bool isValid;
            int s = stateKey.mid(1, stateKey.length() - 1).toInt(&isValid);
            if (!isValid) {
                continue;
            }
            Handlers responders;
            parseSingleState(responders, stateTag);
            stateResponders.insert(s, responders);
        }
    }
    handlers << [this](QWidget* target) {
        auto* std = dynamic_cast<StandardWidget*>(target);
        for (auto& r : globalResponders) {
            std->registerGlobalResponder(r);
        }
        for (auto it = stateResponders.begin() ; it != stateResponders.end() ; it++) {
            for (auto& r : it.value()) {
                std->registerResponder(it.key(), r);
            }
        }
    };
}

void WidgetFactory::parseSingleState(WidgetFactory::Handlers &op, NBT *stateTag) {
    parseSize(op, stateTag);
    parseQss(op, stateTag);
    stdType->onStateRespondersParsing(op, stateTag);
}

void WidgetFactory::parsePointer(NBT *nbt) {
    if (!nbt->getBool("pointer")) {
        return;
    }
    WidgetFactory* root = this;
    while (root->parentFactory) {
        root = root->parentFactory;
    }
    handlers << [root](QWidget* target) {
        auto* rootStd = dynamic_cast<StandardWidget*>(root->workingWidget);
        rootStd->registerPointer(target->objectName(), target);
    };
}

#define parseSize0(key, i) \
    if (nbt->contains(key, Data::INT)) { \
        hasElement[i] = true;   \
        val[i] = nbt->getInt(key);       \
        nothingToDo = false;                       \
    } else {               \
        hasElement[i] = false;                      \
    }\


void WidgetFactory::parseSize(WidgetFactory::Handlers &op, NBT *nbt) {
    bool hasElement[6];
    int val[6];
    bool nothingToDo = true;
    parseSize0("fixed_width", 0)
    parseSize0("fixed_height", 1)
    parseSize0("min_width", 2)
    parseSize0("min_height", 3)
    parseSize0("max_width", 4)
    parseSize0("max_height", 5)
    if (nothingToDo) {
        return;
    }
    op << [hasElement, val](QWidget* target) {
        if (hasElement[0]) {
            target->setFixedWidth(val[0]);
        }
        if (hasElement[1]) {
            target->setFixedHeight(val[1]);
        }
        if (hasElement[2]) {
            target->setMinimumWidth(val[2]);
        }
        if (hasElement[3]) {
            target->setMinimumHeight(val[3]);
        }
        if (hasElement[4]) {
            target->setMaximumWidth(val[4]);
        }
        if (hasElement[5]) {
            target->setMaximumHeight(val[5]);
        }
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
    WidgetFactoryParsers::init();
}

WidgetFactory *WidgetFactory::fromResource(const Identifier &loc) {
    return WidgetFactoryStorage::getInstance()->get(loc);
}

WidgetFactory *WidgetFactory::fromNbt(const QString& id, NBT *nbt) {
    auto* loader = new WidgetFactory(id);
    loader->setSource(nbt);
    return loader;
}

void WidgetFactory::registerStdWidget(const QString &type,
                                      const WidgetFactory::Supplier &supplier,
                                      StandardWidget *instance) {
    if (!customSuppliers->contains(type)) {
        customSuppliers->insert(type, supplier);
        customEmptyInstances->insert(type, instance);
    }
}
