//
// Created by stupid_coder_jyy on 2024/2/27.
//

#include "FocusContainer.h"
#include <QMouseEvent>

FocusContainer* FocusContainer::activated{};

FocusContainer::FocusContainer(QWidget *parent): Widget(parent) {
    setObjectName("c");
    setFocusPolicy(Qt::ClickFocus);
}

void FocusContainer::setContainerState(int focused) {
    if (focused) {
        if (activated == this) {
            return;
        }
        setStyleSheet(getFocusedQss());
        if (activated) {
            activated->setContainerState(false);
        }
        activated = this;
        return;
    }
    setStyleSheet(getNormalQss());
}

FocusContainer::~FocusContainer() {
    if (activated == this) {
        activated = nullptr;
    }
}

QString FocusContainer::getNormalQss() {
    static QString qss = qss_t("c", bg(Styles::CLEAR) + bd("1px", "solid", Styles::GRAY_2));
    return qss;
}

QString FocusContainer::getFocusedQss() {
    static QString qss = qss_t("c", bg(Styles::CLEAR) + bd("2px", "solid", Styles::BLUE_1) + qss("border-radius", "3px"));
    return qss;
}

void FocusContainer::showEvent(QShowEvent *event) {
    setStyleSheet(getNormalQss());
}

void FocusContainer::mousePressEvent(QMouseEvent *event) {
    setContainerState(true);
    event->accept();
}
