//
// Created by stupid_coder_jyy on 2024/4/17.
//

#include "B0.h"

USING_NAMESPACE(lr::log)

B0::B0(QWidget *parent):
        Calendar(new WeekDayTitleDrawer, new CalendarContentDrawer, parent) {
}

void B0::init() {
    Calendar::init();
    setData(new CalendarData);
    loadDate(QDate::currentDate());
}
