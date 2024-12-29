//
// Created by stupid_coder_jyy on 24-11-8.
//

#ifndef CALENDAR_H
#define CALENDAR_H
#include <ListWidget.h>
#include <Namespaces.h>
#include <WidgetData.h>
#include <Common.h>
#include "Button.h"
#include "DropDown.h"
#include "LoadingIcon.h"
#include "SlotsPainter.h"

BEGIN_LR

class CalendarData : public WidgetData {
    friend class Calendar;
    using ViewType = calendar::ViewType;
private:
    ViewType viewType;
    ViewType prevType;
    QDate dateStart;
public:
    CalendarData();
    void set(ViewType viewType, QDate dateStart);
    inline ViewType getViewType() const;
    inline ViewType getPrevType() const;
    inline QDate getDateStart() const;
protected:
    virtual WidgetData* getData(const QDate& date) const;
private:
    void setTopLeft(calendar::ListDataCalendar* ldc) const;
    void gatherData(calendar::ListDataCalendar* ldc) const;
};

class Calendar : public Widget {
private:
    DropDown *dropdownMiniCalendar, *dropdownRange;
    Button *btnPrev, *btnNext, *btnWeek, *btnMonth;
    calendar::MiniCalendarDropDown* miniCalendar;
    Label *labelRange, *labelDate;
    calendar::SlotsWidgetCalendar* slotsContent;
    LoadingIcon* loadingIcon;
    SlotsPainter* spWeekdays;
    calendar::LayerWeekdays* layerWeekdays;
public:
    static void mainInit();
    explicit Calendar(QWidget* parent = nullptr, bool iic = true);
    void setData(WidgetData *d) override;
    void syncDataToWidget() override;
    void connectModelView() override;
    void initWidget() override;
    virtual ListItemCalendar* newItem();
private:
    void refreshContent(CalendarData* cd) const;
    void setOperationEnabled(bool e) const;
};

class ListItemCalendar : public ListItem {
    friend class calendar::SlotsWidgetCalendar;
private:
    calendar::SlotsWidgetCalendar* sw;
    int itemIdx;
    QColor iconColor;
    QFont iconFont;
    int iconNum;
public:
    explicit ListItemCalendar(QWidget* parent = nullptr, bool iic = true);
protected:
    void syncDataToWidget() override;
    void paintEvent(QPaintEvent *event) override;
};

inline CalendarData::ViewType CalendarData::getViewType() const {
    return viewType;
}

inline CalendarData::ViewType CalendarData::getPrevType() const {
    return prevType;
}

inline QDate CalendarData::getDateStart() const {
    return dateStart;
}

END_LR

#endif //CALENDAR_H
