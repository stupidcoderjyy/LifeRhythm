//
// Created by stupid_coder_jyy on 2024/2/27.
//

#include "FocusContainer.h"
#include "FocusManager.h"
#include <QMouseEvent>

FocusContainer::FocusContainer(QWidget *parent): Widget(parent) {
}

void FocusContainer::focusOutEvent(QFocusEvent *event) {
    setStyleSheet(getNormalQss());
    FocusManager::mark(nullptr);
    Widget::focusOutEvent(event);
}

QString FocusContainer::getNormalQss() {
    return qss_this(bg(Styles::CLEAR->rgbHex) + bd("1px", "solid", Styles::GRAY_2->rgbHex) + qss("border-radius", "3px"));
}

QString FocusContainer::getFocusedQss() {
    return qss_this(bg(Styles::CLEAR->rgbHex) + bd("2px", "solid", Styles::BLUE_1->rgbHex) + qss("border-radius", "3px"));
}

void FocusContainer::resizeEvent(QResizeEvent *event) {
    if (styleSheet().isEmpty()) {
        setStyleSheet(getNormalQss());
    }
}

void FocusContainer::mousePressEvent(QMouseEvent *event) {
    setFocus();
    setStyleSheet(getFocusedQss());
    FocusManager::mark(this);
}
