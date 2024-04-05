//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "Calendar.h"
#include "RcManagers.h"
#include <QWheelEvent>

CalendarData::CalendarData(): topLeftDate(), mainMonth(), posMark1(), posMark2() {
}

CalendarData::CalendarData(const QDate &date): topLeftDate(date), mainMonth() {
}

void CalendarData::setTopLeftDate(const QDate &d) {
    QDate date = d;
    if (d.dayOfWeek() != 7) {
        date = d.addDays(1 - d.dayOfWeek());
    }
    if (topLeftDate != date) {
        topLeftDate = date;
        if (date.day() > 14) {
            auto d1 = date.addMonths(1);
            d1.setDate(d1.year(), d1.month(), 1);
            posMark1 = (int)date.daysTo(d1);
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

QStringList WeekDayTitleDrawer::weekDays{"一", "二", "三", "四", "五", "六", "日"};

WeekDayTitleDrawer::WeekDayTitleDrawer(QWidget *parent): AbstractSlotsDrawer(parent) {
    setSlotCount(7, 1);
    setSlotSize(50, 30);
}

void WeekDayTitleDrawer::beforeDrawing(QPainter &p, QRect& area) {
    p.setFont(FontBuilder(Styles::FONT_TYPE_MAIN, Styles::FONT_MAIN).setBoldWeight().get());
    p.fillRect(rect(), Styles::GRAY_TEXT_0->color);
    p.setPen(Styles::BLACK->color);
}

void WeekDayTitleDrawer::drawSlot(QPainter &p, QRect& area, int row, int column) {
    p.drawText(area, Qt::AlignCenter, weekDays[column]);
}

CalendarContentDrawer::CalendarContentDrawer(QWidget *parent): AbstractSlotsDrawer(parent) {
    setSlotCount(7, 6);
    setSlotSize(50, 50);
}

void CalendarContentDrawer::syncDataToWidget() {
    update();
}

void CalendarContentDrawer::beforeDrawing(QPainter &p, QRect &area) {
    if (!wData) {
        return;
    }
    p.setFont(Styles::FONT_LARGE);
    p.setPen(Styles::GRAY_4->color);
    auto* cd = wData->cast<CalendarData>();
    day = cd->topLeftDate.day();
    mainMonthBegin = cd->posMark1;
    mainMonthEnd = cd->posMark2;
    count = 0;
}

void CalendarContentDrawer::drawSlot(QPainter &p, QRect &area, int row, int column) {
    if (!wData) {
        return;
    }
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

void CalendarContentDrawer::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, &CalendarContentDrawer::syncDataToWidget);
}

void CalendarContentDrawer::wheelEvent(QWheelEvent *event) {
    auto* cd = wData->cast<CalendarData>();
    if (event->angleDelta().y() > 0) {
        cd->setTopLeftDate(cd->topLeftDate.addDays(-7));
    } else {
        cd->setTopLeftDate(cd->topLeftDate.addDays(7));
    }
}

Calendar::Calendar(QWidget *parent):
        Widget(parent), shouldInit(true), title(), contentDrawer() {
}

Calendar::~Calendar() {
    delete cd;
}

void Calendar::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<Calendar*>(target)->init();
    };
}

void Calendar::syncDataToWidget() {
    title->setText(cd->mainMonth.toString("yyyy年M月"));
}

void Calendar::loadDate(const QDate &d) {
    cd->setTopLeftDate(d);
}

void Calendar::resizeEvent(QResizeEvent *event) {
    init();
}

void Calendar::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this](){
        syncDataToWidget();
    });
}

void Calendar::init() {
    if (shouldInit) {
        WidgetFactoryStorage::get("widget_calendar")->apply(nullptr, this);
        contentDrawer = getPointer<CalendarContentDrawer>("content");
        title = getPointer<TextLabel>("title");
        buttonPrev = getPointer<ImgButton>("buttonPrev");
        buttonNext = getPointer<ImgButton>("buttonNext");
        connect(buttonPrev, &ImgButton::sigActivated, this, [this](){
            auto d = cd->mainMonth.addMonths(-1);
            d.setDate(d.year(), d.month(), 1);
            loadDate(d.addDays(1 - d.dayOfWeek()));
        });
        connect(buttonNext, &ImgButton::sigActivated, this, [this](){
            auto d = cd->mainMonth.addMonths(1);
            d.setDate(d.year(), d.month(), 1);
            loadDate(d.addDays(1 - d.dayOfWeek()));
        });
        cd = new CalendarData;
        contentDrawer->setData(cd);
        setData(cd);
        shouldInit = false;
        loadDate(QDate::currentDate());
    }
}
