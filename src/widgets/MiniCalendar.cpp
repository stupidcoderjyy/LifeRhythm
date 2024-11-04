//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "MiniCalendar.h"
#include "RcManagers.h"
#include <QWheelEvent>
#include <QLayout>

MiniCalendarData::MiniCalendarData(): topLeftDate(), mainMonth(), posMark1(), posMark2() {
}

MiniCalendarData::MiniCalendarData(const QDate &date): topLeftDate(date), mainMonth() {
}

void MiniCalendarData::setTopLeftDate(const QDate &d) {
    QDate date = d;
    if (d.dayOfWeek() != 1) {
        date = d.addDays(1 - d.dayOfWeek());
    }
    if (topLeftDate != date) {
        topLeftDate = date;
        if (date.day() > 14) {
            auto d1 = date.addMonths(1);
            d1.setDate(d1.year(), d1.month(), 1);
            posMark1 = static_cast<int>(date.daysTo(d1));
            posMark2 = posMark1 + d1.daysInMonth();
            mainMonth = d1;
        } else {
            auto d1 = QDate(date.year(), date.month(), 1);
            posMark1 = 0;
            posMark2 = d1.daysInMonth() - date.day() + 1;
            mainMonth = date;
        }
        emit sigDataChanged();
    }
}

void WeekDayTitleLayer::beforeDrawing(QPainter &p, QRect &area) {
    p.setFont(FontBuilder(Styles::FONT_TYPE_MAIN, Styles::FONT_MAIN).setBoldWeight().get());
    p.fillRect(parent->rect(), Styles::GRAY_0->color);
    p.setPen(Styles::GRAY_TEXT_0->color);
}

void WeekDayTitleLayer::drawSlot(QPainter &p, QRect &area, int row, int column) {
    p.drawText(area, Qt::AlignCenter, MiniCalendar::WEEKDAYS_CN[column]);
}

CalendarContentLayer::CalendarContentLayer(): firstDay(), mainMonthBegin(-1), mainMonthEnd(-1), day(), count() {
}

bool CalendarContentLayer::shouldDraw() {
    return parent->widgetData();
}

void CalendarContentLayer::beforeDrawing(QPainter &p, QRect &area) {
    p.setFont(Styles::FONT_LARGE);
    p.setPen(Styles::GRAY_4->color);
    day = firstDay;
    count = 0;
}

void CalendarContentLayer::drawSlot(QPainter &p, QRect &area, int row, int column) {
    if (count == mainMonthBegin) {
        p.setPen(Styles::GRAY_TEXT_0->color);
        if (mainMonthBegin > 0) {
            day = 1;
        }
    } else if (count == mainMonthEnd) {
        p.setPen(Styles::GRAY_3->color);
        day = 1;
    }
    p.drawText(area, Qt::AlignCenter, QString::number(day));
    day++;
    count++;
}


WeekDayTitleDrawer::WeekDayTitleDrawer(QWidget *parent): SlotsPainter(50, 40, 7, 1, parent) {
}

void WeekDayTitleDrawer::initLayers() {
    appendLayer(new WeekDayTitleLayer);
}

CalendarContentDrawer::CalendarContentDrawer(QWidget *parent):
        SlotsPainter(50, 50, 7, 6, parent), baseLayer(new CalendarContentLayer) {
}

void CalendarContentDrawer::initLayers() {
    appendLayer(baseLayer);
}

void CalendarContentDrawer::wheelEvent(QWheelEvent *event) {
    auto* cd = wData->cast<MiniCalendarData>();
    if (!cd) {
        return;
    }
    if (event->angleDelta().y() > 0) {
        cd->setTopLeftDate(cd->topLeftDate.addDays(-7));
    } else {
        cd->setTopLeftDate(cd->topLeftDate.addDays(7));
    }
}

const QStringList MiniCalendar::WEEKDAYS_CN{"一", "二", "三", "四", "五", "六", "日"};
const QSize MiniCalendar::SIZE(350, 380);

MiniCalendar::MiniCalendar(WeekDayTitleDrawer* t, CalendarContentDrawer* c, QWidget *parent):
        Widget(parent), shouldInit(true), title(), contentDrawer(c), titleDrawer(t) {
}

void MiniCalendar::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<MiniCalendar *>(target)->initCalendar();
    };
}

void MiniCalendar::syncDataToWidget() {
    if (wData) {
        auto* cd = wData->cast<MiniCalendarData>();
        title->setText(cd->mainMonth.toString("yyyy年M月"));
        contentDrawer->baseLayer->firstDay = cd->topLeftDate.day();
        contentDrawer->baseLayer->mainMonthBegin = cd->posMark1;
        contentDrawer->baseLayer->mainMonthEnd = cd->posMark2;
        contentDrawer->update();
    }
}

void MiniCalendar::loadDate(const QDate &d) {
    if (wData) {
        wData->cast<MiniCalendarData>()->setTopLeftDate(d);
    }
}

void MiniCalendar::setData(WidgetData *d) {
    auto* cd = dynamic_cast<MiniCalendarData*>(d);
    if (cd) {
        Widget::setData(d);
        contentDrawer->setData(d);
    }
}

void MiniCalendar::initCalendar() {
    if (shouldInit) {
        init();
        shouldInit = false;
    }
}

void MiniCalendar::resizeEvent(QResizeEvent *event) {
    initCalendar();
}

void MiniCalendar::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this](){
        syncDataToWidget();
    });
}

void MiniCalendar::init() {
    WidgetFactoryStorage::get("widget_calendar")->apply(nullptr, this);
    titleDrawer->setParent(this);
    contentDrawer->setParent(this);
    layout()->addWidget(titleDrawer);
    layout()->addWidget(contentDrawer);
    title = getPointer<TextLabel>("title");
    buttonPrev = getPointer<ImgButton>("buttonPrev");
    buttonNext = getPointer<ImgButton>("buttonNext");
    connect(buttonPrev, &ImgButton::sigActivated, this, [this](){
        if (wData) {
            auto* cd = wData->cast<MiniCalendarData>();
            auto d = cd->mainMonth.addMonths(-1);
            d.setDate(d.year(), d.month(), 1);
            cd->setTopLeftDate(d.addDays(1 - d.dayOfWeek()));
        }
    });
    connect(buttonNext, &ImgButton::sigActivated, this, [this](){
        if (wData) {
            auto* cd = wData->cast<MiniCalendarData>();
            auto d = cd->mainMonth.addMonths(1);
            d.setDate(d.year(), d.month(), 1);
            cd->setTopLeftDate(d.addDays(1 - d.dayOfWeek()));
        }
    });
}