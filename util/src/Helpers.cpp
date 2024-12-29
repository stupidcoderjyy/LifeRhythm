//
// Created by JYY on 24-12-24.
//

#include "Helpers.h"
#include <QMouseEvent>
#include <QLayout>
#include <QPainter>
#include <QPainterPath>

USING_LR

const QStringList Helpers::WEEKDAYS_CN{"一", "二", "三", "四", "五", "六", "日"};

int **Helpers::allocateArray(int rows, int columns) {
    auto p = new int *[rows];
    for (int i = 0; i < rows; i++) {
        p[i] = new int[columns];
        memset(p[i], 0, columns << 2);
    }
    return p;
}

int *Helpers::allocateArray(int size) {
    auto p = new int[size];
    memset(p, 0, size << 2);
    return p;
}

void Helpers::freeArray(int **p, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] p[i];
    }
    delete[] p;
}

bool Helpers::isHovered(QWidget *widget, QMouseEvent *evt) {
    return isHovered(widget, widget->mapToParent(evt->pos()));
}

bool Helpers::isHovered(QWidget *widget, const QPoint &pos) {
    return widget->geometry().contains(pos);
}

bool Helpers::isMouseHovered(QWidget *widget) {
    if (widget->parentWidget()) {
        auto rect = widget->rect();
        auto gp = widget->parentWidget()->mapToGlobal(widget->pos());
        rect.setRect(gp.x(), gp.y(), rect.width(), rect.height());
        return rect.contains(QCursor::pos());
    }
    return widget->rect().contains(widget->mapFromGlobal(QCursor::pos()));
}

QPoint Helpers::getGlobalPos(QWidget *widget) {
    if (widget->parentWidget()) {
        return widget->parentWidget()->mapToGlobal(widget->pos());
    }
    return widget->pos();
}

void Helpers::moveWidget(QWidget *widget, const QPoint &pos) {
    moveWidget(widget, pos.x(), pos.y());
}

void Helpers::moveWidget(QWidget *widget, int x, int y) {
    auto rect = widget->rect();
    rect.setX(x);
    rect.setY(y);
    widget->setGeometry(rect);
}

void Helpers::switchSingleLayoutWidget(QLayout *layout, QWidget *pre, QWidget *cur) {
    if (pre) {
        layout->removeWidget(pre);
        pre->setParent(nullptr);
        pre->hide();
    }
    layout->addWidget(cur);
    cur->setParent(layout->parentWidget());
    cur->show();
}

void Helpers::fillRoundedRect(QPainter &painter, const QRect &rect, int radius, const QBrush &color) {
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);
    painter.fillPath(path, color);
    painter.setRenderHint(QPainter::Antialiasing, false);
}
