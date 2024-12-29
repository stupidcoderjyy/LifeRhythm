//
// Created by stupid_coder_jyy on 2024/2/28.
//

#include "ArrowButton.h"
#include "NBT.h"
#include "Error.h"
#include <QResizeEvent>
#include <QPainter>
#include "ImageStorage.h"

USING_LR

ArrowButton::ArrowButton(QWidget *parent, bool initInConstructor): Button(parent, initInConstructor),
                                                                   baseType(Down), activatedType(Down), showType(None) {
    setButtonMode(SelectClick);
}

void ArrowButton::setActivatedType(Type t) {
    if (activatedType != t) {
        activatedType = t;
        if (selected) {
            updateBase();
        }
    }
}

void ArrowButton::setBaseType(Type t) {
    if (baseType != t) {
        baseType = t;
        if (!selected) {
            updateBase();
        }
    }
}

void ArrowButton::setSelected(bool selected) {
    Button::setSelected(selected);
    updateBase();
}

void ArrowButton::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    Button::onPostParsing(handlers, widgetTag);
    Type b = None, a = None;
    if (widgetTag->contains("base", Data::STRING)) {
        b = fromString(widgetTag->getString("base"));
    }
    if (widgetTag->contains("activated", Data::STRING)) {
        a = fromString(widgetTag->getString("activated"));
    }
    handlers << [a, b](QWidget *w) {
        auto ab = static_cast<ArrowButton*>(w);
        ab->setActivatedType(a);
        ab->setBaseType(b);
    };
}

void ArrowButton::initWidget() {
    updateBase();
}

void ArrowButton::updateBase() {
    auto rArrow = ImageStorage::get("lr:arrow_30x30");
    Type to = selected ? activatedType : baseType;
    if (showType != to) {
        QTransform t;
        t.rotate(to * 90);
        if (enabled) {
            setButtonImg(rArrow->transformed(t), imgDisabled);
        } else {
            setButtonImg(imgNormal, rArrow->transformed(t));
        }
        showType = to;
    }
}

ArrowButton::Type ArrowButton::fromString(const QString &str) {
    if (str == "Up") {
        return Up;
    }
    if (str == "Down") {
        return Down;
    }
    if (str == "Left") {
        return Left;
    }
    if (str == "Right") {
        return Right;
    }
    return None;
}