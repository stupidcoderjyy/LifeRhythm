//
// Created by JYY on 24-12-13.
//

#ifndef CALENDARINCLUDES_H
#define CALENDARINCLUDES_H

#include "Namespaces.h"
#include <QDate>

BEGIN_LR

class ListItemCalendar;
class Calendar;
class CalendarData;

BEGIN_NP(calendar)

enum ViewType {
    Month, D1, D2, D3, D4, D5, D6, D7
};

class LayerDay;
class LayerMonth;
class MiniCalendarDropDown;
class SlotsWidgetCalendar;
class LayerWeekdays;

class ListDataCalendar : public ListData {
public:
    QDate topLeft;
    ViewType viewType;
};

END_NP

END_LR

#endif //CALENDARINCLUDES_H
