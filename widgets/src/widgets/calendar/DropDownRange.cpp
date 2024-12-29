//
// Created by JYY on 24-12-13.
//

#include "DropDownRange.h"
#include "Calendar.h"

USING_NP(lr::calendar)

DataRange::DataRange(int type): days(type) {
}

ItemRange::ItemRange(QWidget *parent, bool iic): ListItem(parent, iic), label() {
}

void ItemRange::initWidget() {
    if (!prepared) {
        label = getPointer<Label>("l");
        prepared = true;
    }
}

void ItemRange::syncDataToWidget() {
    if (auto md = wData->cast<DataRange>()) {
        label->setText(QString::number(md->days) + "天");
    }
}

void ItemRange::enterEvent(QEnterEvent *event) {
    setState(1);
}

void ItemRange::leaveEvent(QEvent *event) {
    setState(0);
}

DropDownRange::DropDownRange(QWidget *parent, bool iic): DropDown(parent, iic), label(), list(), viewType(),
                                                         optionsData(new ListData) {
}

void DropDownRange::syncWidgetToData() {
    if (auto cd = wData->cast<CalendarData>()) {
        cd->set(viewType, cd->getDateStart());
    }
}

void DropDownRange::initWidget() {
    DropDown::initWidget();
    label = getPointer<Label>("l");
    list = getPointer<ListWidget>("list");
    for (int t = 1; t <= 7; t++) {
        optionsData->append(new DataRange(t));
    }
    list->setRowHeight(29);
    list->setFixedHeight(29 * 7);
    list->setData(optionsData);
    connect(optionsData, &ListData::sigDataSelected, this, [this](int, int cur) {
        viewType = static_cast<ViewType>(cur + 1);
        optionsData->selectData(-1);
        syncWidgetToData();
        emit sigCloseMenu();
    });
}

DropDownRange::~DropDownRange() {
    delete optionsData;
}