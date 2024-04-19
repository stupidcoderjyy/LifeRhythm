//
// Created by stupid_coder_jyy on 2024/4/17.
//

#include <QPainterPath>
#include "DateRangeSelector.h"
#include "DRSData.h"
#include <QMouseEvent>
#include <QHBoxLayout>

USING_NAMESPACE(lr::log)

DateRangeSelector::DateRangeSelector(QWidget *parent): AbstractOptionsBox(parent), calendar() {
    auto* l = new QHBoxLayout(this);
    setLayout(l);
    l->setContentsMargins(10,0,10,0);
    labelRange = new TextLabel(this);
    l->addWidget(labelRange);
}

void DateRangeSelector::syncDataToWidget() {
    if (wData) {
        auto* dc = wData->cast<DRSData>();
        labelRange->setText(dc->getRangeText());
    } else {
        labelRange->setText("--");
    }
}

void DateRangeSelector::initMenu(OptionsMenu *menu) {
    auto* l = new QHBoxLayout(menu);
    QMargins margins = {2,2,2,2};
    menu->setFixedSize(Calendar::SIZE.grownBy(margins));
    menu->setLayout(l);
    calendar = new DRSCalendar(menu);
    l->setContentsMargins(margins);
    l->addWidget(calendar);
    setData(new DRSData);
}

void DateRangeSelector::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this](){
        syncDataToWidget();
    });
}

void DateRangeSelector::setData(WidgetData *d) {
    Widget::setData(d);
    calendar->setData(d);
    calendar->initCalendar();
}

DRSCalendar::DRSCalendar(QWidget *parent):
        Calendar(new WeekDayTitleDrawer, new DRSCalendarDrawer, parent) {
}

void DRSCalendar::syncDataToWidget() {
    if (wData) {
        auto* cd = wData->cast<DRSData>();
        auto* drsDrawer = static_cast<DRSCalendarDrawer*>(contentDrawer);
        drsDrawer->drsLayer->begin = cd->getBeginPos();
        drsDrawer->drsLayer->end = cd->getEndPos();
        Calendar::syncDataToWidget();
    }
}

void DRSCalendar::init() {
    Calendar::init();
    loadDate(QDate::currentDate());
}

DRSCalendarDrawer::DRSCalendarDrawer(QWidget *parent): CalendarContentDrawer(parent), drsLayer(new DRSCalendarLayer) {
}

void DRSCalendarDrawer::initLayers() {
    appendLayer(drsLayer);
    appendLayer(baseLayer);
}

void DRSCalendarDrawer::onSlotClicked(QMouseEvent* evt, int row, int column) {
    if (wData) {
        if (evt->button() == Qt::LeftButton) {
            wData->cast<DRSData>()->selectDate(true, row, column);
        } else if (evt->button() == Qt::RightButton) {
            wData->cast<DRSData>()->selectDate(false, row, column);
        }
    }
}

void DRSCalendarDrawer::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.fillRect(rect(), Styles::BLACK->color);
    SlotsDrawer::paintEvent(event);
}

#define BASE_Y 40
#define RADIUS 3

DRSCalendarLayer::DRSCalendarLayer(): count(), begin(-1), end(-1) {
}

void DRSCalendarLayer::beforeDrawing(QPainter &p, QRect &area) {
    count = 0;
    inRange = begin < 0;
    p.setPen(Styles::BLUE_1->color);
}

void DRSCalendarLayer::drawSlot(QPainter &p, QRect &area, int row, int column) {
    if (count == begin) {
        QPainterPath path;
        QPoint center(area.x() + 25, area.y() + BASE_Y);
        path.addEllipse(center, RADIUS, RADIUS);
        if (end > 0 && count != end) {
            p.fillRect(center.x(), center.y() - RADIUS, 25, RADIUS * 2, Styles::BLUE_0->color);
            inRange = true;
        }
        p.fillPath(path, Styles::BLUE_1->color);
    } else if (count == end) {
        QPainterPath path;
        QPoint center(area.x() + 25, area.y() + BASE_Y);
        path.addEllipse(center, RADIUS, RADIUS);
        p.fillRect(area.x(), center.y() - RADIUS, 25, RADIUS * 2, Styles::BLUE_0->color);
        p.fillPath(path, Styles::BLUE_1->color);
        inRange = false;
    } else if (inRange) {
        p.fillRect(area.x(), area.y() + BASE_Y - RADIUS, 50, RADIUS * 2, Styles::BLUE_0->color);
    }
    count++;
}

bool DRSCalendarLayer::shouldDraw() {
    return begin >= 0 || end >= 0;
}
