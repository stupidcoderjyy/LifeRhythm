//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "StdImgButton.h"
#include "Styles.h"

StdImgButton::StdImgButton(QWidget *parent) : StdImgLabel(parent) {
    QString qssBorder = qss("border-style", "none") + qss("border-radius", "2px");
    qssNormal = qssbg(Styles::BLACK) + qssBorder;
    qssActivated = qssbg(Styles::GRAY_0) + qssBorder;
}

void StdImgButton::enterEvent(QEvent *event) {
    setStyleSheet(qssActivated);
}

void StdImgButton::mouseReleaseEvent(QMouseEvent *ev) {
    StdImgLabel::mouseReleaseEvent(ev);
    setStyleSheet(qssActivated);
}

void StdImgButton::leaveEvent(QEvent *event) {
    setStyleSheet(qssNormal);
}
