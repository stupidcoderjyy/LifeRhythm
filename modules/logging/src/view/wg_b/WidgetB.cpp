//
// Created by stupid_coder_jyy on 2024/4/15.
//

#include "WidgetB.h"
#include "Period.h"

USING_NAMESPACE(lr::log)

WidgetB::WidgetB(QWidget *parent): TabWidget(parent) {
}

void WidgetB::onTabCreated() {
    wv = getPointer<WeekView>("wv");
    auto* nd = new NestedListData;
    wv->setData(nd);
    nd->makeRow(7);
    nd->append(0, new Period(4 * 60, 5 * 60, "aaaaa"));
    nd->append(1, new Period(5 * 60, 8 * 60, "bbbbbb"));
    nd->markAll();
    wvTitle = getPointer<WeekViewTitle>("wvTitle");
}
