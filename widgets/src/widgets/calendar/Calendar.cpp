//
// Created by stupid_coder_jyy on 24-11-8.
//

#include "Calendar.h"
#include "AsyncTask.h"
#include "ArrowButton.h"
#include "MiniCalendarDropDown.h"
#include "ButtonSwitchView.h"
#include "SlotsWidgetCalendar.h"
#include "DropDownRange.h"
#include "SlotsPainterWeekdays.h"
#include "WidgetFactoryStorage.h"

USING_LR

CalendarData::CalendarData(): viewType(ViewType::D3), prevType(), dateStart(QDate::currentDate()) {
}

void CalendarData::set(ViewType vt, QDate ds) {
    if (vt == ViewType::Month && viewType != ViewType::Month) {
        prevType = viewType;
    }
    viewType = vt;
    dateStart = ds;
    emit sigDataChanged();
}

WidgetData * CalendarData::getData(const QDate &date) const {
    return nullptr;
}

void CalendarData::setTopLeft(calendar::ListDataCalendar *ldc) const {
    if (viewType == calendar::Month) {
        QDate d = dateStart;
        d.setDate(d.year(), d.month(), 1);
        ldc->topLeft = d.addDays(1 - d.dayOfWeek());
    } else {
        ldc->topLeft = dateStart;
    }
}

void CalendarData::gatherData(calendar::ListDataCalendar* ldc) const {
    int len = viewType == calendar::Month ? 42 : viewType;
    for (int i = 0; i < len; i++) {
        ldc->append(getData(ldc->topLeft.addDays(i)));
    }
}

void Calendar::mainInit() {
    auto f = WidgetFactoryStorage::get("test:widget_mini_calendar");
    regClazz(f, Widget);
    regClazz(f, Button);
    regClazz(f, ArrowButton);
    f = WidgetFactoryStorage::get("test:calendar/widget_calendar");
    regClazz(f, Widget);
    regClazz(f, calendar::MiniCalendarDropDown);
    regClazz(f, DropDown);
    regClazz(f, Calendar);
    regClazz(f, ArrowButton);
    regClazz(f, Button);
    regClazz(f, LoadingIcon);
    regClazz(f, calendar::ButtonSwitchView);
    regClazz(f, calendar::SlotsWidgetCalendar);
    regClazz(f, SlotsPainter);
    f = WidgetFactoryStorage::get("test:calendar/dropdown_range");
    regClazz(f, Widget);
    regClazz(f, calendar::DropDownRange);
    regClazz(f, Label);
    regClazz(f, ArrowButton);
    regClazz(f, ListWidget);
    regClazz(f, calendar::ItemRange);
    f = WidgetFactoryStorage::get("test:calendar/dropdown_mini_calendar");
    regClazz(f, Widget);
    regClazz(f, calendar::MiniCalendarDropDown);
    regClazz(f, DropDown);
    regClazz(f, Label);
}

Calendar::Calendar(QWidget *parent, bool iic): Widget(parent, iic), dropdownMiniCalendar(), dropdownRange(),
                                               btnPrev(), btnNext(), btnWeek(), btnMonth(), miniCalendar(),
                                               labelRange(), labelDate(), slotsContent(), loadingIcon(),
                                               spWeekdays(), layerWeekdays() {
}

void Calendar::setData(WidgetData *d) {
    if (auto cd = dynamic_cast<CalendarData*>(d)) {
        Widget::setData(d);
        miniCalendar->setData(cd);
        dropdownRange->setData(cd);
    }
}

void Calendar::syncDataToWidget() {
    if (auto* cd = wData->cast<CalendarData>()) {
        miniCalendar->syncDataToWidget();
        labelRange->setText(QString::number(cd->getViewType()) + "天");
        labelRange->setSizeToText();
        QString s;
        switch (cd->getViewType()) {
            case calendar::Month: {
                s = cd->getDateStart().toString("yyyy年MM月");
                break;
            }
            case calendar::D1: {
                s = cd->getDateStart().toString("yyyy年 MM月dd日");
                break;
            }
            default: {
                auto d = cd->getDateStart(), d1 = d.addDays(cd->getViewType() - 1);
                if (d.year() == d1.year()) {
                    s = d.toString("yyyy年 MM月dd日");
                    s += " ~ ";
                    s += d1.toString("MM月dd日");
                } else {
                    s = d.toString("yyyy年MM月dd日");
                    s += " ~ ";
                    s += d1.toString("yyyy年MM月dd日");
                }
                break;
            }
        }
        labelDate->setText(s);
        labelDate->setSizeToText();
        bool isMonth = cd->getViewType() == calendar::Month;
        dropdownRange->setVisible(!isMonth);
        btnWeek->setSelected(!isMonth);
        btnMonth->setSelected(isMonth);
        if (isMonth) {
            slotsContent->setSlotCount(7, 6);
            spWeekdays->setSlotCount(7, 1);
            layerWeekdays->set(0);
        } else {
            slotsContent->setSlotCount(cd->getViewType(), 1);
            spWeekdays->setSlotCount(cd->getViewType(), 1);
            layerWeekdays->set(cd->getDateStart().dayOfWeek() - 1);
        }
        slotsContent->cd = cd;
        refreshContent(cd);
    }
}

void Calendar::connectModelView() {
    auto* cd = wData->cast<CalendarData>();
    dc << connect(cd, &WidgetData::sigDataChanged, this, &Calendar::syncDataToWidget);
    dc << connect(miniCalendar, &MiniCalendar::sigRelease, this, [this, cd] {
        switch (miniCalendar->getViewLevel()) {
            case mini_calendar::Day: {
                miniCalendar->syncWidgetToData();
                emit dropdownMiniCalendar->sigCloseMenu();
                break;
            }
            case mini_calendar::Month: {
                if (cd->getViewType() == calendar::Month) {
                    miniCalendar->syncWidgetToData();
                    emit dropdownMiniCalendar->sigCloseMenu();
                }
                break;
            }
            default:
                break;
        }
    });
    dc << connect(dropdownMiniCalendar, &DropDown::sigMenuOpening, this, [this, cd] {
        if (cd->getViewType() == calendar::Month) {
            miniCalendar->setViewLevel(mini_calendar::Month);
        } else {
            miniCalendar->setViewLevel(mini_calendar::Day);
        }
        miniCalendar->loadDate(cd->getDateStart());
    });
    dc << connect(btnPrev, &Button::sigSelected, this, [cd] {
        if (cd->getViewType() == calendar::Month) {
            cd->set(cd->getViewType(), cd->getDateStart().addMonths(-1));
        } else {
            cd->set(cd->getViewType(), cd->getDateStart().addDays(-cd->getViewType()));
        }
    });
    dc << connect(btnNext, &Button::sigSelected, this, [cd] {
        if (cd->getViewType() == calendar::Month) {
            cd->set(cd->getViewType(), cd->getDateStart().addMonths(1));
        } else {
            cd->set(cd->getViewType(), cd->getDateStart().addDays(cd->getViewType()));
        }
    });
    dc << connect(btnWeek, &Button::sigSelected, this, [cd](bool s) {
        if (s) {
            cd->set(cd->getPrevType(), cd->getDateStart());
        }
    });
    dc << connect(btnMonth, &Button::sigSelected, this, [cd](bool s) {
        if (s) {
            cd->set(calendar::Month, cd->getDateStart());
        }
    });
}

void Calendar::refreshContent(CalendarData* cd) const {
    auto ldc = slotsContent->widgetData()->cast<calendar::ListDataCalendar>();
    DELETE_LIST(ldc->getData())
    ldc->clear();
    cd->setTopLeft(ldc);
    slotsContent->syncItems();
    auto getter = AsyncTask::create();
    getter->setTask([cd, ldc] {
        cd->gatherData(ldc);
    });
    getter->setFinished(this, [this] {
        slotsContent->widgetData()->cast<ListData>()->markAll();
        setOperationEnabled(true);
    });
    getter->start();
    setOperationEnabled(false);
}

void Calendar::setOperationEnabled(bool e) const {
    btnPrev->setWidgetEnabled(e);
    btnNext->setWidgetEnabled(e);
    btnWeek->setWidgetEnabled(e);
    btnMonth->setWidgetEnabled(e);
    dropdownRange->setWidgetEnabled(e);
    dropdownMiniCalendar->setWidgetEnabled(e);
    if (e) {
        loadingIcon->stopLoading();
    } else {
        loadingIcon->startLoading();
    }
}

void Calendar::initWidget() {
    if (!prepared) {
        WidgetFactoryStorage::get("test:calendar/widget_calendar")->apply(nullptr, this);
        dropdownMiniCalendar = getPointer<DropDown>("dropdown_mini_calendar");
        labelDate = dropdownMiniCalendar->getPointer<Label>("l");
        miniCalendar = dropdownMiniCalendar->getPointer<calendar::MiniCalendarDropDown>("c");
        dropdownRange = getPointer<calendar::DropDownRange>("dropdown_range");
        labelRange = dropdownRange->getPointer<Label>("l");
        btnPrev = getPointer<Button>("btnPrev");
        btnNext = getPointer<Button>("btnNext");
        btnWeek = getPointer<Button>("btnWeek");
        btnMonth = getPointer<Button>("btnMonth");
        btnWeek->setSelected(true);
        slotsContent = getPointer<calendar::SlotsWidgetCalendar>("slots");
        slotsContent->setSlotMargins(0,0,1,1);
        slotsContent->setRegionMargins(1,1,0,0);
        slotsContent->calendar = this;
        slotsContent->setData(new calendar::ListDataCalendar);
        loadingIcon = getPointer<LoadingIcon>("loadingIcon");
        layerWeekdays = new calendar::LayerWeekdays;
        spWeekdays = getPointer<SlotsPainter>("weekdays");
        spWeekdays->addLayer(layerWeekdays);
        prepared = true;
    }
}

ListItemCalendar * Calendar::newItem() {
    return new ListItemCalendar();
}

ListItemCalendar::ListItemCalendar(QWidget *parent, bool iic): ListItem(parent, iic), sw(), itemIdx(), iconNum() {
}

void ListItemCalendar::syncDataToWidget() {
    ListItem::syncDataToWidget();
    if (!sw->cd) {
        return;
    }
    auto topLeft = listData->cast<calendar::ListDataCalendar>()->topLeft;
    auto d = topLeft.addDays(itemIdx);
    if (sw->cd->getViewType() == calendar::Month) {
        int m = sw->cd->getDateStart().month();
        if (d.month() == m) {
            iconColor = Styles::GRAY_TEXT_1->color;
        } else {
            iconColor = Styles::GRAY_4->color;
        }
        if (d == QDate::currentDate()) {
            iconColor = Styles::BLUE_1->color;
            iconFont = FontBuilder(Styles::FONT_TYPE_MAIN, Styles::FONT_SMALL).setBoldWeight().get();
        } else {
            iconFont = Styles::FONT_SMALL;
        }
    } else {
        auto dd = d.daysTo(QDate::currentDate());
        if (dd < 0) {
            iconFont = Styles::FONT_SMALL;
            iconColor = Styles::GRAY_TEXT_1->color;
        } else if (dd > 0) {
            iconFont = Styles::FONT_SMALL;
            iconColor = Styles::GRAY_4->color;
        } else {
            iconFont = FontBuilder(Styles::FONT_TYPE_MAIN, Styles::FONT_SMALL).setBoldWeight().get();
            iconColor = Styles::BLUE_1->color;
        }
    }
    iconNum = d.day();
    update();
}

void ListItemCalendar::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.setFont(iconFont);
    p.setPen(iconColor);
    auto str = QString::number(iconNum);
    auto strSize = Label::baseTextSize(str, p.font());
    p.drawText(QRect(0, 0, strSize.width(), strSize.height()), str, {Qt::AlignCenter});
}