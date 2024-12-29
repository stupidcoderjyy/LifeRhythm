//
// Created by JYY on 24-12-13.
//

#ifndef SLOTSWIDGETCALENDAR_H
#define SLOTSWIDGETCALENDAR_H

#include <Namespaces.h>
#include <SlotsWidget.h>
#include <Common.h>

BEGIN_NP(lr::calendar)

class SlotsWidgetCalendar : public SlotsWidget {
    friend class lr::ListItemCalendar;
    friend class lr::Calendar;
private:
    CalendarData *cd;
    Calendar* calendar;
public:
    explicit SlotsWidgetCalendar(QWidget *parent = nullptr, bool iic = true);
    ~SlotsWidgetCalendar() override;
protected:
    ListItem *newItem() override;
    void paintEvent(QPaintEvent *event) override;
    void updateBase() override;
};

END_NP

#endif //SLOTSWIDGETCALENDAR_H
