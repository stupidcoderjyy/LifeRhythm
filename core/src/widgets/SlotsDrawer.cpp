//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "SlotsDrawer.h"
#include "MemUtil.h"
#include <QMouseEvent>

SlotsDrawer::SlotsDrawer(int slotsWidth, int slotsHeight, int columns, int rows, QWidget *parent):
        Widget(parent), layers(), init(), slotWidth(slotsWidth),
        slotHeight(slotsHeight), columns(columns), rows(rows) {
    setFixedSize(slotWidth * columns, slotHeight * rows);
}

SlotsDrawer::~SlotsDrawer() {
    DELETE_LIST(layers)
}

void SlotsDrawer::paintEvent(QPaintEvent *event) {
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

void SlotsDrawer::resizeEvent(QResizeEvent *event) {
    if (!init) {
        initLayers();
        init = true;
    }
}

void SlotsDrawer::mousePressEvent(QMouseEvent *event) {
    auto p = event->pos();
    onSlotClicked(event, p.y() / slotHeight, p.x() / slotWidth);
}

void SlotsDrawer::onSlotClicked(QMouseEvent* evt, int row, int column) {
}

DrawerLayer::DrawerLayer(): parent() {
}

bool DrawerLayer::shouldDraw() {
    return true;
}

void DrawerLayer::beforeDrawing(QPainter &p, QRect &area) {
}

void DrawerLayer::drawSlot(QPainter &p, QRect &area, int row, int column) {
}

void DrawerLayer::afterDrawing(QPainter &p) {
}
