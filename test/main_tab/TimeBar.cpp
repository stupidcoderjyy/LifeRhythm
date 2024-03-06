//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "TimeBar.h"
#include "PeriodWidget.h"
#include "RcManagers.h"
#include "ListData.h"

TimeBar::TimeBar(QWidget *parent): RangeBar(true, parent) {
    setBarRange(0, 1440);
    setZoomRange(0.5, 3);
    setZoomStep(0.5);
}

RangeWidget *TimeBar::createRangeWidget() {
    static auto* f = WidgetFactoryStorage::get("log:period");
    return static_cast<RangeWidget*>(f->apply());
}
