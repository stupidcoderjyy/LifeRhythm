//
// Created by stupid_coder_jyy on 2024/4/17.
//

#include "DRSData.h"

USING_NAMESPACE(lr::log)

DRSData::DRSData(): CalendarData(), rangeBegin(), rangeEnd() {
}

QString DRSData::getRangeText() {
    if (rangeBegin.isNull()) {
        return "--";
    }
    QString res = rangeBegin.toString("yyyy年M月d日");
    if (rangeEnd.isNull()) {
        return res;
    }
    return res + " - " + rangeEnd.toString("yyyy年M月d日");
}

void DRSData::selectDate(bool begin, int row, int column) {
    QDate d = topLeftDate.addDays(row * 7 + column);
    if (rangeBegin.isNull() || (begin && rangeEnd.isNull())) {
        rangeBegin = d;
        rangeEnd = {};
        emit sigDataChanged();
        return;
    }
    QDate d1 = begin ? rangeEnd : rangeBegin;
    if (d1 > d) {
        rangeBegin = d;
        rangeEnd = d1;
    } else if (d1 < d) {
        rangeBegin = d1;
        rangeEnd = d;
    } else {
        rangeBegin = d;
        rangeEnd = {};
    }
    emit sigDataChanged();
}
