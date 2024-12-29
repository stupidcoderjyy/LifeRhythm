//
// Created by stupid_coder_jyy on 2024/2/27.
//

#include "FocusContainer.h"
#include "FocusManager.h"
#include <QMouseEvent>

USING_LR

FocusContainer::FocusContainer(QWidget *parent, bool initInConstructor): Widget(parent, initInConstructor) {
    setObjectName("fc");
}

void FocusContainer::focusOutEvent(QFocusEvent *event) {
    setStyleSheet(getNormalQss());
    FocusManager::mark(nullptr);
    Widget::focusOutEvent(event);
}

QString FocusContainer::getNormalQss() {
    return qss_this(bg(Styles::CLEAR->rgbHex) + bd("1px", "solid", Styles::GRAY_3->rgbHex) + brad("3px"));
}

QString FocusContainer::getFocusedQss() {
    return qss_this(bg(Styles::CLEAR->rgbHex) + bd("2px", "solid", Styles::BLUE_1->rgbHex) + brad("3px"));
}

void FocusContainer::initWidget() {
    setStyleSheet(getNormalQss());
}

void FocusContainer::mousePressEvent(QMouseEvent *event) {
    setFocus();
    setStyleSheet(getFocusedQss());
    FocusManager::mark(this);
}
