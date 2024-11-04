//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef LIFERHYTHM_CALENDAR_H
#define LIFERHYTHM_CALENDAR_H

#include "Widget.h"
#include "TextLabel.h"
#include "ImgButton.h"
#include "SlotsPainter.h"
#include <QDateTime>

class CORE_API MiniCalendarData : public WidgetData {
    friend class CalendarContentDrawer;
    friend class MiniCalendar;
protected:
    QDate topLeftDate;
    QDate mainMonth;
    int posMark1;
    int posMark2;
public:
    MiniCalendarData();
    explicit MiniCalendarData(const QDate &date);
    void setTopLeftDate(const QDate& d);
};

class CORE_API WeekDayTitleLayer : public DrawerLayer {
protected:
    void beforeDrawing(QPainter &p, QRect &area) override;
    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
};

class CORE_API CalendarContentLayer : public DrawerLayer {
    friend class MiniCalendar;
private:
    int firstDay;
    int mainMonthBegin;
    int mainMonthEnd;
    int day;
    int count;
public:
    CalendarContentLayer();
protected:
    bool shouldDraw() override;
    void beforeDrawing(QPainter &p, QRect &area) override;
    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
};

class CORE_API WeekDayTitleDrawer : public SlotsPainter {
public:
    explicit WeekDayTitleDrawer(QWidget* parent = nullptr);
protected:
    void initLayers() override;
};

class CORE_API CalendarContentDrawer : public SlotsPainter {
    friend class MiniCalendar;
protected:
    CalendarContentLayer* baseLayer;
public:
    explicit CalendarContentDrawer(QWidget* parent = nullptr);
protected:
    void initLayers() override;
    void wheelEvent(QWheelEvent *event) override;
};

class CORE_API MiniCalendar : public Widget {
public:
    static const QStringList WEEKDAYS_CN;
    static const QSize SIZE;
protected:
    CalendarContentDrawer* contentDrawer;
    WeekDayTitleDrawer* titleDrawer;
    ImgButton* buttonPrev;
    ImgButton* buttonNext;
    TextLabel* title;
    bool shouldInit;
public:
    explicit MiniCalendar(
            WeekDayTitleDrawer* t,
            CalendarContentDrawer* c,
            QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void syncDataToWidget() override;
    void loadDate(const QDate& d);
    void setData(WidgetData *d) override;
    void initCalendar();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void connectModelView() override;
    virtual void init();
};

#endif //LIFERHYTHM_CALENDAR_H
