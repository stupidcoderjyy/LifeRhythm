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

bool isMouseHovered(QWidget *widget) {
    if (widget->parentWidget()) {
        auto rect = widget->rect();
        auto gp = widget->parentWidget()->mapToGlobal(widget->pos());
        rect.setRect(gp.x(), gp.y(), rect.width(), rect.height());
        return rect.contains(QCursor::pos());
    }
    return widget->rect().contains(widget->mapFromGlobal(QCursor::pos()));
}

QPoint getGlobalPos(QWidget *widget) {
    if (widget->parentWidget()) {
        return widget->parentWidget()->mapToGlobal(widget->pos());
    }
    return widget->pos();
}

void moveWidget(QWidget *widget, const QPoint &pos) {
    moveWidget(widget, pos.x(), pos.y());
}

void moveWidget(QWidget *widget, int x, int y) {
    auto rect = widget->rect();
    rect.setX(x);
    rect.setY(y);
    widget->setGeometry(rect);
}
