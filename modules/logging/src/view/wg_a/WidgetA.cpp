//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "WidgetA.h"
#include "Period.h"

USING_NAMESPACE(lr::log)

WidgetA::WidgetA(QWidget *parent): TabWidget(parent) {
}

void WidgetA::onTabCreated() {
    bar = getPointer<TimeBar>("timeBar");
    auto* d = new NestedListData;
    d->makeRow();
    d->append(0, new Period(4 * 60, 5 * 60, "aaaaa"));
    d->append(0, new Period(5 * 60, 8 * 60, "bbbbbb"));
    bar->setData(d);
    d->markAll();
}
