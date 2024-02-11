//
// Created by stupid_coder_jyy on 2024/2/11.
//

#include "WidgetUtil.h"

bool isHovered(QWidget *widget, QMouseEvent *evt) {
    return isHovered(widget, widget->mapToParent(evt->pos()));
}

bool isHovered(QWidget *widget, const QPoint &pos) {
    return widget->geometry().contains(pos);
}

