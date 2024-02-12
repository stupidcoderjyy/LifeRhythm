//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "widgets/ImgButton.h"
#include "Styles.h"
#include "WidgetUtil.h"
#include "QMouseEvent"

ImgButton::ImgButton(QWidget *parent) : ImgLabel(parent) {
    QString qssBorder = qss("border-style", "none") + qss("border-radius", "2px");
    qssNormal = bg(Styles::BLACK) + qssBorder;
    qssActivated = bg(Styles::GRAY_0) + qssBorder;
}

void ImgButton::enterEvent(QEvent *event) {
    setStyleSheet(qssActivated);
}

void ImgButton::mouseReleaseEvent(QMouseEvent *ev) {
    if (isHovered(this, ev)) {
        emit sigActivated();
        setStyleSheet(qssActivated);
    }
}

void ImgButton::leaveEvent(QEvent *event) {
    setStyleSheet(qssNormal);
}
