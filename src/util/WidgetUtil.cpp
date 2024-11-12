//
// Created by stupid_coder_jyy on 2024/2/11.
//

#include "WidgetUtil.h"
#include <QMouseEvent>
#include <QLayout>
#include <QPainter>
#include <QPainterPath>

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

void switchSingleLayoutWidget(QLayout *layout, QWidget *pre, QWidget *cur) {
    if (pre) {
        layout->removeWidget(pre);
        pre->setParent(nullptr);
        pre->hide();
    }
    layout->addWidget(cur);
    cur->setParent(layout->parentWidget());
    cur->show();
}

void fillRoundedRect(QPainter &painter, const QRect &rect, int radius, const QBrush& color) {
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);
    painter.fillPath(path, color);
    painter.setRenderHint(QPainter::Antialiasing, false);
}
