//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "SlotsPainter.h"
#include "MemUtil.h"
#include <QMouseEvent>

SlotsPainter::SlotsPainter(int slotsWidth, int slotsHeight, int columns, int rows, QWidget *parent):
        Widget(parent), layers(), init(), slotWidth(slotsWidth),
        slotHeight(slotsHeight), columns(columns), rows(rows) {
    setFixedSize(slotWidth * columns, slotHeight * rows);
}

SlotsPainter::~SlotsPainter() {
    DELETE_LIST(layers)
}

void SlotsPainter::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    QRect area;
    for (auto* l : layers) {
        if (!l->shouldDraw()) {
            continue;
        }
        area.setRect(0, 0, slotWidth, slotHeight);
        l->beforeDrawing(p, area);
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                l->drawSlot(p, area, r, c);
                area.translate(slotWidth, 0);
            }
            area.translate(-area.x(), slotHeight);
        }
        l->afterDrawing(p);
    }
}

void SlotsPainter::resizeEvent(QResizeEvent *event) {
    if (!init) {
        initLayers();
        init = true;
    }
}

void SlotsPainter::mousePressEvent(QMouseEvent *event) {
    auto p = event->pos();
    onSlotClicked(event, p.y() / slotHeight, p.x() / slotWidth);
}

void SlotsPainter::onSlotClicked(QMouseEvent* evt, int row, int column) {
}

DrawerLayer::DrawerLayer(): parent() {
}

DrawerLayer::~DrawerLayer() = default;

bool DrawerLayer::shouldDraw() {
    return true;
}

void DrawerLayer::beforeDrawing(QPainter &p, QRect &area) {
}

void DrawerLayer::drawSlot(QPainter &p, QRect &area, int row, int column) {
}

void DrawerLayer::afterDrawing(QPainter &p) {
}
