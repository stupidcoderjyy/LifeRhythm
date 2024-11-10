//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "ImgButton.h"
#include "Styles.h"
#include "WidgetUtil.h"
#include "QMouseEvent"
#include "FocusManager.h"

ImgButton::ImgButton(QWidget *parent) : ImgLabel(parent) {
    QString qssBorder = qss("border-style", "none") + qss("border-radius", "2px");
    qssNormal = bg(Styles::CLEAR->rgbHex) + qssBorder;
    qssHovered = bg(Styles::GRAY_1->rgbHex) + qssBorder;
    qssPressed = bg(Styles::GRAY_2->rgbHex) + qssBorder;
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void ImgButton::enterEvent(QEvent *event) {
    setStyleSheet(qssHovered);
}

void ImgButton::mouseReleaseEvent(QMouseEvent *ev) {
    if (isHovered(this, ev)) {
        emit sigActivated();
        setStyleSheet(qssHovered);
    }
}

void ImgButton::mousePressEvent(QMouseEvent *ev) {
    setStyleSheet(qssPressed);
    FocusManager::mark(this);
}

void ImgButton::leaveEvent(QEvent *event) {
    setStyleSheet(qssNormal);
}
