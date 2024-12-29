//
// Created by JYY on 24-12-14.
//

#include "SlotsPainterWeekdays.h"
#include "Helpers.h"

USING_NP(lr::calendar)

LayerWeekdays::LayerWeekdays(): begin() {
}

void LayerWeekdays::set(int b) {
    begin = b;
    parent->update();
}

void LayerWeekdays::beforeDrawing(QPainter &p) {
    p.setFont(FontBuilder(Styles::FONT_TYPE_MAIN, Styles::FONT_SMALL).setBoldWeight().get());
    p.fillRect(parent->rect(), Styles::BLACK->color);
    p.setPen(Styles::GRAY_4->color);
}

void LayerWeekdays::drawSlot(QPainter &p, QRect &area, int column, int row) {
    p.drawText(area, Qt::AlignLeft | Qt::AlignBottom, "周" + Helpers::WEEKDAYS_CN[(begin + column) % 7]);
}
