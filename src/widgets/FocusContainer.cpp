//
// Created by stupid_coder_jyy on 2024/2/27.
//

#include "FocusContainer.h"
#include "FocusManager.h"
#include <QMouseEvent>

FocusContainer::FocusContainer(QWidget *parent): Widget(parent) {
}

void FocusContainer::focusInEvent(QFocusEvent *event) {
    setStyleSheet(getFocusedQss());
}

void FocusContainer::focusOutEvent(QFocusEvent *event) {
    setStyleSheet(getNormalQss());
}

QString FocusContainer::getNormalQss() {
    return qss_this(bg(Styles::CLEAR) + bd("1px", "solid", Styles::GRAY_2));
}

QString FocusContainer::getFocusedQss() {
    return qss_this(bg(Styles::CLEAR) + bd("2px", "solid", Styles::BLUE_1) + qss("border-radius", "3px"));
}

void FocusContainer::mousePressEvent(QMouseEvent *event) {
    FocusManager::markFocus(this);
}