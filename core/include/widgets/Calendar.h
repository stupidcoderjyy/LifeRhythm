//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef LIFERHYTHM_CALENDAR_H
#define LIFERHYTHM_CALENDAR_H

#include "Widget.h"
#include "TextLabel.h"
#include "ImgButton.h"
#include "AbstractSlotsDrawer.h"
#include <QDateTime>

class CalendarData : public WidgetData {
    friend class CalendarContentDrawer;
    friend class Calendar;
private:
    QDate topLeftDate;
    QDate mainMonth;
    int posMark1;
    int posMark2;
public:
    CalendarData();
    explicit CalendarData(const QDate &date);
    void setTopLeftDate(const QDate& d);
};

class WeekDayTitleDrawer : public AbstractSlotsDrawer {
private:
    static QStringList weekDays;
public:
    explicit WeekDayTitleDrawer(QWidget* parent = nullptr);
protected:
    void beforeDrawing(QPainter &p, QRect& area) override;
    void drawSlot(QPainter &p, QRect& area, int row, int column) override;
};

class CalendarContentDrawer : public AbstractSlotsDrawer {
private:
    int day;
    int count;
    int mainMonthBegin;
    int mainMonthEnd;
public:
    explicit CalendarContentDrawer(QWidget* parent = nullptr);
    void syncDataToWidget() override;
protected:
    void beforeDrawing(QPainter &p, QRect& area) override;
    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
    void connectModelView() override;
    void wheelEvent(QWheelEvent *event) override;
};

class Calendar : public Widget {
private:
    CalendarData* cd;
    CalendarContentDrawer* contentDrawer;
    ImgButton* buttonPrev;
    ImgButton* buttonNext;
    TextLabel* title;
    bool shouldInit;
public:
    explicit Calendar(CalendarContentDrawer* content, QWidget* parent = nullptr);
    ~Calendar() override;
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void syncDataToWidget() override;
    void loadDate(const QDate& d);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void connectModelView() override;
    virtual void init();
};

inline void AbstractSlotsDrawer::setSlotSize(int w, int h) {
    if (slotWidth != w || slotHeight != h) {
        slotWidth = w;
        slotHeight = h;
        shouldResize = true;
    }
}

inline void AbstractSlotsDrawer::setSlotCount(int c, int r) {
    if (columns != c || rows != r) {
        columns = c;
        rows = r;
        shouldResize = true;
    }
}

#endif //LIFERHYTHM_CALENDAR_H
