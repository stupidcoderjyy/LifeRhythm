//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "StdImgButton.h"
#include "Styles.h"
#include "WidgetUtil.h"
#include "QMouseEvent"

StdImgButton::StdImgButton(QWidget *parent) : StdImgLabel(parent) {
    QString qssBorder = qss("border-style", "none") + qss("border-radius", "2px");
    qssNormal = bg(Styles::BLACK) + qssBorder;
    qssActivated = bg(Styles::GRAY_0) + qssBorder;
}

void StdImgButton::enterEvent(QEvent *event) {
    setStyleSheet(qssActivated);
}

void StdImgButton::mouseReleaseEvent(QMouseEvent *ev) {
    if (isHovered(this, ev)) {
        emit sigActivated();
        setStyleSheet(qssActivated);
    }
}

void StdImgButton::leaveEvent(QEvent *event) {
    setStyleSheet(qssNormal);
}
