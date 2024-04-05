//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "AbstractSlotsDrawer.h"

AbstractSlotsDrawer::AbstractSlotsDrawer(QWidget *parent):
        Widget(parent), slotWidth(40), slotHeight(40), columns(0), rows(0), shouldResize(true) {
}

void AbstractSlotsDrawer::beforeDrawing(QPainter &p, QRect& area) {
}

void AbstractSlotsDrawer::afterDrawing(QPainter &p) {
}

void AbstractSlotsDrawer::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    QRect area(0, 0, slotWidth, slotHeight);
    p.save();
    beforeDrawing(p, area);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            drawSlot(p, area, r, c);
            area.translate(slotWidth, 0);
        }
        area.translate(-area.x(), slotHeight);
    }
    afterDrawing(p);
    p.restore();
}

void AbstractSlotsDrawer::resizeEvent(QResizeEvent *event) {
    if (shouldResize) {
        setFixedSize(slotWidth * columns, slotHeight * rows);
    }
}