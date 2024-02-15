//
// Created by stupid_coder_jyy on 2024/1/17.
//

#include "WidgetFactoryParsers.h"
#include "NBT.h"
#include "Error.h"

QMap<QString, Qt::AlignmentFlag> WidgetFactoryParsers::alignments{};
QMap<QString, QSizePolicy::Policy> WidgetFactoryParsers::policies{};

Qt::Alignment WidgetFactoryParsers::parseAlign(const QString &alignment) {
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

void WidgetFactoryParsers::parseSizePolicy(WidgetFactory::Handlers& handlers, NBT* nbt) {
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

QMargins WidgetFactoryParsers::parseMargins(ArrayData *array) {
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

void WidgetFactoryParsers::init() {
    alignments.insert("Left",Qt::AlignLeft);
    alignments.insert("Right",Qt::AlignRight);
    alignments.insert("Top",Qt::AlignTop);
    alignments.insert("Bottom",Qt::AlignBottom);
    alignments.insert("V_Center",Qt::AlignVCenter);
    alignments.insert("H_Center",Qt::AlignHCenter);
    alignments.insert("Center",Qt::AlignCenter);

    policies.insert("Min",QSizePolicy::Minimum);
    policies.insert("Normal",QSizePolicy::Preferred);
    policies.insert("Max",QSizePolicy::Maximum);
    policies.insert("Fixed",QSizePolicy::Fixed);
    policies.insert("Ignored",QSizePolicy::Ignored);
    policies.insert("MinExpand",QSizePolicy::MinimumExpanding);
}

QSize WidgetFactoryParsers::parseSize(ArrayData *arr) {
    auto data = arr->get();
    if (data.length() != 2) {
        throwInFunc("requires int[2]");
    }
    for (int i = 0 ; i < 2 ; i ++) {
        if (data[i]->type != Data::INT) {
            throwInFunc("invalid data type, requires int");
        }
    }
    return {data[0]->asInt()->get(), data[1]->asInt()->get()};
}
