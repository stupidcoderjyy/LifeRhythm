//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "TimeBar.h"
#include "RcManagers.h"
#include "ListData.h"
#include "Period.h"

TimeBarItem::TimeBarItem(QWidget *parent): RangeBarItem(parent), labelInfo(), labelRange() {
}

void TimeBarItem::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<TimeBarItem*>(target)->init();
    };
}

void TimeBarItem::init() {
    labelInfo = getPointer<TextLabel>("labelInfo");
    labelRange = getPointer<TextLabel>("labelRange");
}

void TimeBarItem::syncDataToWidget() {
    RangeBarItem::syncDataToWidget();
    if (wData) {
        show();
        auto* d = wData->cast<Period>();
        labelInfo->setText(d->info);
        labelRange->setText(QString::asprintf("%02d:%02d-%02d:%02d", d->begin / 60, d->begin % 60, d->end / 60, d->end % 60));
    } else {
        close();
    }
}

TimeBar::TimeBar(QWidget *parent): RangeBar(true, parent) {
    setBarRange(0, 1440);
    setZoomRange(0.5, 3);
    setZoomStep(0.5);
}

RangeBarItem *TimeBar::createRangeWidget() {
    static auto* f = WidgetFactoryStorage::get("log:item_timebar");
    return static_cast<RangeBarItem*>(f->apply());
}
