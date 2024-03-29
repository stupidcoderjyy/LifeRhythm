//
// Created by stupid_coder_jyy on 2024/2/23.
//

#include "DragWidget.h"
#include "Error.h"
#include <QMouseEvent>

WidgetDragger::WidgetDragger(QObject* parent):QObject(parent) {
    memset(allowDrag, true, 4);
}

void WidgetDragger::setEnabled(WidgetDragger::Orientation o, bool b) {
    if (o != Center) {
        allowDrag[o] = b;
    }
}

void WidgetDragger::setTarget(QWidget *w) {
    target = w;
}


void WidgetDragger::onMousePressed(QMouseEvent* evt) {
    startPos = evt->pos();
    mousePressed = true;
}

void WidgetDragger::onMouseReleased() {
    mousePressed = false;
}

void WidgetDragger::onMouseMoved(QMouseEvent* evt) {
    if (mousePressed) {
        QPoint delta = evt->pos() - startPos;
        applyDrag(hOrientation, dragDistance(hOrientation, delta.x()));
        applyDrag(vOrientation, dragDistance(vOrientation, delta.y()));
    } else {
        checkHor();
        checkVer();
    }
}

int WidgetDragger::dragDistance(WidgetDragger::Orientation o, int delta) {
    return delta;
}

bool WidgetDragger::shouldDrag(WidgetDragger::Orientation o) {
    return o != Center && allowDrag[o];
}

void WidgetDragger::checkHor() {
    if (startPos.x() < 4) {
        hOrientation = Left;
    } else if (startPos.x() > target->width() - 4) {
        hOrientation = Right;
    }
    if (shouldDrag(hOrientation)) {
        target->setCursor(Qt::SizeHorCursor);
    }
}

void WidgetDragger::checkVer() {
    if (startPos.y() < 4) {
        vOrientation = Top;
    } else if (startPos.y() > target->height() - 4) {
        vOrientation = Bottom;
    }
    if (shouldDrag(vOrientation)) {
        target->setCursor(Qt::SizeVerCursor);
    }
}

void WidgetDragger::applyDrag(WidgetDragger::Orientation o, int delta) {
    if (!shouldDrag(o) || !delta) {
        return;
    }
    QRect rect = target->rect();
    switch (o) {
        case Left:
            rect.setX(rect.x() + delta);
            rect.setWidth(rect.width() + delta);
            break;
        case Top:
            rect.setY(rect.y() + delta);
            rect.setHeight(rect.height() + delta);
            break;
        case Right:
            rect.setWidth(rect.width() + delta);
            break;
        case Bottom:
            rect.setHeight(rect.height() + delta);
            break;
        case Center:
            break;
    }
    target->setGeometry(rect);
}