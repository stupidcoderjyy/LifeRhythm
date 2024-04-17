//
// Created by stupid_coder_jyy on 2024/4/17.
//

#ifndef LIFERHYTHM_DRSDATA_H
#define LIFERHYTHM_DRSDATA_H

#include "Calendar.h"
#include "Namespaces.h"

BEGIN_NAMESPACE(lr::log)

class DRSData : public CalendarData {
    friend class DRSCalendar;
    friend class DRSCalendarDrawer;
private:
    QDate rangeBegin, rangeEnd;
public:
    DRSData();
    QString getRangeText();
private:
    void selectDate(bool begin, int row, int column);
    inline int getBeginPos();
    inline int getEndPos();
};

inline int DRSData::getBeginPos() {
    if (rangeBegin.isNull()) {
        return -1;
    }
    return (int)topLeftDate.daysTo(rangeBegin);
}

inline int DRSData::getEndPos() {
    if (rangeEnd.isNull()) {
        return -1;
    }
    return (int)topLeftDate.daysTo(rangeEnd);
}

END_NAMESPACE

#endif //LIFERHYTHM_DRSDATA_H
