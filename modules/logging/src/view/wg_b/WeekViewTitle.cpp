//
// Created by stupid_coder_jyy on 2024/4/15.
//

#include "WeekViewTitle.h"
#include "WeekViewData.h"

USING_NAMESPACE(lr::log)

WeekViewTitle::WeekViewTitle(QWidget *parent): SlotsDrawer(parent) {
    setSlotSize(200, 40);
    setSlotCount(7, 1);
    memset(months, 0, 28);
    memset(days, 0, 28);
}

void WeekViewTitle::syncDataToWidget() {
    if (!wData) {
        return;
    }
    QDate d = wData->cast<WeekViewData>()->weekBegin;
    for (int i = 0 ; i < 7 ; i ++) {
        d = d.addDays(i);
        months[i] = d.month();
        days[i] = d.day();
    }
    update();
}

void WeekViewTitle::initLayers() {
    appendLayer(new WeekViewTitleLayer(months, days));
}

WeekViewTitleLayer::WeekViewTitleLayer(int* months, int* days): months(months), days(days) {
}

void WeekViewTitleLayer::beforeDrawing(QPainter &p, QRect &area) {
    p.setFont(Styles::FONT_MAIN);
    p.setPen(Styles::GRAY_TEXT_0->color);
}

void WeekViewTitleLayer::drawSlot(QPainter &p, QRect &area, int row, int column) {
    p.drawText(area, Qt::AlignCenter,
               QString::number(months[column]) + "月" + QString::number(days[column]) + "日");
}
