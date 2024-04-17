//
// Created by stupid_coder_jyy on 2024/4/17.
//

#ifndef LIFERHYTHM_DATERANGESELECTOR_H
#define LIFERHYTHM_DATERANGESELECTOR_H

#include "Calendar.h"
#include "AbstractOptionsBox.h"
#include "Namespaces.h"
#include "TextLabel.h"

BEGIN_NAMESPACE(lr::log)

class DRSCalendarDrawer;
class DRSCalendarLayer;
class DRSCalendar;
class DRSData;

class DateRangeSelector : public AbstractOptionsBox {
private:
    DRSData* data;
    DRSCalendar* calendar;
    TextLabel* labelRange;
public:
    explicit DateRangeSelector(QWidget* parent = nullptr);
    void syncDataToWidget() override;
    void setData(WidgetData *d) override;
protected:
    void initMenu(OptionsMenu *menu) override;
    void connectModelView() override;
};

class DRSCalendar : public Calendar {
public:
    explicit DRSCalendar(QWidget* parent = nullptr);
    void syncDataToWidget() override;
protected:
    void init() override;
};

class DRSCalendarDrawer : public CalendarContentDrawer {
    friend class DRSCalendar;
private:
    DRSCalendarLayer* drsLayer;
public:
    explicit DRSCalendarDrawer(QWidget* parent = nullptr);
protected:
    void initLayers() override;
    void onSlotClicked(QMouseEvent* evt, int row, int column) override;
    void paintEvent(QPaintEvent *event) override;
};

class DRSCalendarLayer : public DrawerLayer {
    friend class DRSCalendar;
private:
    int begin;
    int end;
    int count;
    bool inRange;
public:
    DRSCalendarLayer();
protected:
    void beforeDrawing(QPainter &p, QRect &area) override;
    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
    bool shouldDraw() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_DATERANGESELECTOR_H
