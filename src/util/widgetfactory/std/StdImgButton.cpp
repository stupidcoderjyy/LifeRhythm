//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "StdImgButton.h"
#include "Styles.h"

StdImgButton::StdImgButton(QWidget *parent) : StdImgLabel(parent) {
    QString qssBorder = qss("border-style", "none") + qss("border-radius", "2px");
    qssNormal = qssbg(Styles::BLACK) + qssBorder;
    qssHovered = qssbg(Styles::GRAY_0) + qssBorder;
    qssSelected = qssbg(Styles::GRAY_1) + qssBorder;
}

void StdImgButton::enterEvent(QEvent *event) {
    setStyleSheet(qssHovered);
}

void StdImgButton::mousePressEvent(QMouseEvent *ev) {
    setStyleSheet(qssSelected);
}

void StdImgButton::mouseReleaseEvent(QMouseEvent *ev) {
    StdImgLabel::mouseReleaseEvent(ev);
    setStyleSheet(qssHovered);
}

void StdImgButton::leaveEvent(QEvent *event) {
    setStyleSheet(qssNormal);
}
