//
// Created by stupid_coder_jyy on 2024/4/18.
//

#include "B2.h"
#include "UsageInfo.h"

USING_NAMESPACE(lr::log)

TimeUsageItem::TimeUsageItem(QWidget *parent):
        ListItem(parent), shouldDraw(), barColor(Styles::CYAN_0->color), usage(), usageName(), usagePerc() {
}

void TimeUsageItem::syncDataToWidget() {
    shouldDraw = wData;
    if (!wData) {
        return;
    }
    auto* tu = wData->cast<UsageInfo>();
    usagePerc = tu->usagePerc;
    usageName = tu->usageName;
    usage = tu->usage;
    update();
}

void TimeUsageItem::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    QRect r = rect();
    p.fillRect(r, Styles::BLACK->color);
    if (!shouldDraw) {
        return;
    }
    r.adjust(2, 2, -2, -2);
    p.fillRect(r.x(), r.y(), r.width() * usagePerc / 100, r.height(), barColor);
    r.adjust(3, 0, -7, 0);
    p.setFont(Styles::FONT_MAIN);
    p.setPen(Styles::GRAY_TEXT_0->color);
    p.drawText(r, Qt::AlignLeft | Qt::AlignVCenter, usageName);
    p.drawText(r, Qt::AlignRight | Qt::AlignVCenter, usage);
}

B2::B2(QWidget *parent): ListWidget(parent) {
}

ListItem *B2::createRowItem() {
    return new TimeUsageItem;
}
