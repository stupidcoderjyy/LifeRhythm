//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef LIFERHYTHM_CALENDAR_H
#define LIFERHYTHM_CALENDAR_H

#include "Widget.h"
#include "TextLabel.h"
#include "ImgButton.h"
#include "SlotsDrawer.h"
#include <QDateTime>

class CalendarData : public WidgetData {
    friend class CalendarContentLayer;
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

class WeekDayTitleLayer : public DrawerLayer {
protected:
    void beforeDrawing(QPainter &p, QRect &area) override;
    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
};

class CalendarContentLayer : public DrawerLayer {
private:
    int day;
    int count;
    int mainMonthBegin;
    int mainMonthEnd;
protected:
    bool shouldDraw() override;
    void beforeDrawing(QPainter &p, QRect &area) override;
    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
};

class WeekDayTitleDrawer : public SlotsDrawer {
protected:
    void initLayers() override;
};

class CalendarContentDrawer : public SlotsDrawer {
public:
    explicit CalendarContentDrawer(QWidget* parent = nullptr);
    void syncDataToWidget() override;
protected:
    void initLayers() override;
    void connectModelView() override;
    void wheelEvent(QWheelEvent *event) override;
};

class Calendar : public Widget {
public:
    static const QStringList WEEKDAYS_CN;
protected:
    CalendarData* cd;
    CalendarContentDrawer* contentDrawer;
    WeekDayTitleDrawer* titleDrawer;
    ImgButton* buttonPrev;
    ImgButton* buttonNext;
    TextLabel* title;
private:
    bool shouldInit;
public:
    explicit Calendar(
            WeekDayTitleDrawer* t,
            CalendarContentDrawer* c,
            QWidget* parent = nullptr);
    ~Calendar() override;
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void syncDataToWidget() override;
    void loadDate(const QDate& d);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void connectModelView() override;
    virtual void init();
};

#endif //LIFERHYTHM_CALENDAR_H
