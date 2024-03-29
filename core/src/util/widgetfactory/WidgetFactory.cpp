//
// Created by stupid_coder_jyy on 2024/1/14.
//
#include "WidgetFactory.h"
#include "MemUtil.h"
#include "NBT.h"
#include "RcManagers.h"
#include "QssParser.h"
#include "FactoryInit.h"
#include "WidgetDataStorage.h"
#include "GridLayout.h"

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
        parseModel(source);
        parseQss(handlers,source);
        parseLayout(source);
        parseSize(handlers, source);
        parseStates(source);
        parseSizePolicy(handlers, source);
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
        loader = WidgetFactoryStorage::get(loc);
        if (!loader) {
            throwInFunc("factory not found '" + loc.toString() + "'");
        }
    }
    //组件类型，如果使用外部加载器，则stdType会被忽略（别手贱写个不存在的type）
    QString type = nbt->getString("type", "Widget");
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
        if (auto* gl = dynamic_cast<GridLayout*>(layout)) {
            gl->appendWidget(workingWidget);
        } else {
            layout->addWidget(workingWidget);
        }
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
        auto* loader = WidgetFactoryStorage::get(loaderLoc);
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
    bool absolute = nbt->contains("qss_a", Data::STRING);
    if (!nbt->contains("qss", Data::STRING) && !absolute) {
        return;
    }
    QString qss = QssParser::translate(absolute ? nbt->getString("qss_a") : nbt->getString("qss"));
    op << [qss, absolute](QWidget* target) {
        if (absolute) {
            target->setStyleSheet(qss);
        } else {
            target->setStyleSheet('#' + target->objectName() + '{' + qss + '}');
        }
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
    } else if (type == "Grid") {
        parseGridLayout(nbt);
        return;
    } else {
        throwInFunc("invalid layout type '" + type + "'");
    }
    Qt::Alignment alignment{};
    if (nbt->contains("align", Data::STRING)) {
        alignment = parseAlign(nbt->get("align")->asString()->get());
    }
    QMargins margins{};
    if (nbt->contains("margins", Data::ARR)) {
        margins = parseMargins(nbt->get("margins")->asArray());
    }
    int spacing = 5;
    if (nbt->contains("spacing", Data::INT)) {
        spacing = nbt->get("spacing")->asInt()->get();
    }

    handlers << [layoutType, alignment, margins, spacing](QWidget* target) {
        QLayout* layout;
        if (layoutType) {
            layout = new QVBoxLayout(target);
        } else {
            layout = new QHBoxLayout(target);
        }
        layout->setAlignment(alignment);
        layout->setContentsMargins(margins);
        layout->setSpacing(spacing);
        target->setLayout(layout);
    };
}

void WidgetFactory::parseGridLayout(NBT *nbt) {
    int maxColumns = nbt->getInt("max_columns", 1);
    Qt::Alignment align = Qt::AlignCenter;
    if (nbt->contains("align", Data::STRING)) {
        align = parseAlign(nbt->getString("align"));
    }
    QVector<Qt::Alignment> aligns{};
    if (nbt->contains("column_aligns", Data::ARR)) {
        QStringList list;
        nbt->get("column_aligns")->asArray()->fillString(list, maxColumns, "Center");
        for (auto& s : list) {
            aligns << parseAlign(s);
        }
    }
    QMargins margins{};
    if (nbt->contains("margins", Data::ARR)) {
        margins = parseMargins(nbt->get("margins")->asArray());
    }
    int columnSpacing = nbt->getInt("column_spacing");
    int rowSpacing = nbt->getInt("row_spacing");
    QVector<int> rowMinHeights, columnMinWidths, rowStretches, columnStretches;
    if (nbt->contains("row_heights", Data::ARR)) {
        nbt->get("row_heights")->asArray()->fillInt(rowMinHeights);
    }
    if (nbt->contains("column_widths", Data::ARR)) {
        nbt->get("column_widths")->asArray()->fillInt(columnMinWidths, maxColumns);
    }
    if (nbt->contains("row_stretches", Data::ARR)) {
        nbt->get("row_stretches")->asArray()->fillInt(rowStretches);
    }
    if (nbt->contains("column_stretches", Data::ARR)) {
        nbt->get("column_stretches")->asArray()->fillInt(columnStretches, maxColumns);
    }
    handlers << [maxColumns, align, aligns, margins, columnSpacing, rowSpacing,
                        rowMinHeights, columnMinWidths, rowStretches, columnStretches](QWidget* target) {
        auto* l = new GridLayout(target, maxColumns);
        for (int i = 0 ; i < rowMinHeights.length() ; i ++) {
            l->setRowMinimumHeight(i, rowMinHeights[i]);
        }
        for (int i = 0 ; i < columnMinWidths.length() ; i ++) {
            l->setColumnMinimumWidth(i, columnMinWidths[i]);
        }
        for (int i = 0 ; i < rowStretches.length() ; i ++) {
            l->setRowStretch(i, rowStretches[i]);
        }
        for (int i = 0 ; i < columnStretches.length() ; i ++) {
            l->setColumnStretch(i, columnStretches[i]);
        }
        l->columnAlign = aligns;
        l->setAlignment(align);
        l->setContentsMargins(margins);
        l->setHorizontalSpacing(columnSpacing);
        l->setVerticalSpacing(rowSpacing);
        target->setLayout(l);
    };
}

void WidgetFactory::parseMargins(NBT *nbt) {
    if (!nbt->contains("margins", Data::ARR)) {
        return;
    }
    QMargins margins = parseMargins(nbt->get("margins")->asArray());
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
            int s = stateKey.midRef(1, stateKey.length() - 1).toInt(&isValid);
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

Qt::Alignment WidgetFactory::parseAlign(const QString &alignment) {
    if (alignment.isEmpty()) {
        return Qt::AlignCenter;
    }
    auto flags = alignment.split('|');
    Qt::Alignment result{};
    for (auto& f : flags) {
        if (alignments.contains(f)) {
            result = result | alignments.value(f);
        } else {
            throwInFunc("invalid alignment flag '" + f + "'");
        }
    }
    return result;
}

void WidgetFactory::parseSizePolicy(WidgetFactory::Handlers& handlers, NBT* nbt) {
    bool hasH = nbt->contains("h_size_policy", Data::STRING);
    bool hasV = nbt->contains("v_size_policy", Data::STRING);
    QSizePolicy::Policy h, v;
    if (!hasH && !hasV) {
        return;
    }
    if (hasH) {
        auto hs = nbt->getString("h_size_policy");
        if (policies.contains(hs)) {
            h = policies.value(hs);
        } else {
            throwInFunc("invalid size policy:" + hs);
        }
    }
    if (hasV) {
        auto vs = nbt->getString("v_size_policy");
        if (policies.contains(vs)) {
            v = policies.value(vs);
        } else {
            throwInFunc("invalid size policy:" + vs);
        }
    }
    handlers << [hasH, hasV, h, v](QWidget* target) {
        auto policy = target->sizePolicy();
        auto realH = hasH ? h : policy.horizontalPolicy();
        auto realV = hasV ? v : policy.verticalPolicy();
        target->setSizePolicy(realH,realV);
    };
}

QSize WidgetFactory::parseSize(ArrayData *arr) {
    int data[2]{0,0};
    arr->fillInt(data, 2);
    return {data[0], data[1]};
}

QMargins WidgetFactory::parseMargins(ArrayData *array) {
    auto elements = array->get();
    if (elements.size() != 4) {
        throwInFunc("invalid margin array");
    }
    int arr[4];
    for (int i = 0 ; i < 4 ; i ++) {
        if (elements[i]->type != Data::INT) {
            throwInFunc("invalid data type in margin array");
        }
        arr[i] = elements[i]->asInt()->get();
    }
    return {arr[0], arr[1], arr[2], arr[3]};
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

QWidget *WidgetFactory::createWidget(const QString &type, QWidget *parent) {
    if (stdSuppliers.contains(type)) {
        return stdSuppliers.value(type)(parent);
    }
    if (customSuppliers->contains(type)) {
        return customSuppliers->value(type)(parent);
    }
    return nullptr;
}

void WidgetFactory::parseModel(NBT *nbt) {
    if (nbt->contains("model", Data::STRING)) {
        WidgetData* d = WidgetDataStorage::get(Identifier(nbt->getString("model")));
        if (!d) {
            throwInFunc("failed to find widget data '" + nbt->getString("model") + "'");
        }
        handlers << [d](QWidget* target) {
            dynamic_cast<StandardWidget*>(target)->setData(d);
        };
    }
}

QMap<QString, WidgetFactory::Supplier> WidgetFactory::stdSuppliers{};
QMap<QString, StandardWidget*> WidgetFactory::stdEmptyInstances{};
QMap<QString, Qt::AlignmentFlag> WidgetFactory::alignments{};
QMap<QString, QSizePolicy::Policy> WidgetFactory::policies{};

void WidgetFactory::mainInit() {
    FactoryInit::mainInit(&stdSuppliers, &stdEmptyInstances);
    QssParser::mainInit();
    alignments.insert("Left",Qt::AlignLeft);
    alignments.insert("Right",Qt::AlignRight);
    alignments.insert("Top",Qt::AlignTop);
    alignments.insert("Bottom",Qt::AlignBottom);
    alignments.insert("V_Center",Qt::AlignVCenter);
    alignments.insert("H_Center",Qt::AlignHCenter);
    alignments.insert("Center",Qt::AlignCenter);
    policies.insert("Min",QSizePolicy::Minimum);
    policies.insert("Preferred",QSizePolicy::Preferred);
    policies.insert("Max",QSizePolicy::Maximum);
    policies.insert("Fixed",QSizePolicy::Fixed);
    policies.insert("Ignored",QSizePolicy::Ignored);
    policies.insert("Expanding",QSizePolicy::Expanding);
}

WidgetFactory *WidgetFactory::fromResource(const Identifier &loc) {
    return WidgetFactoryStorage::get(loc);
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
