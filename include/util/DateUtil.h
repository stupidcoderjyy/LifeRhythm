//
// Created by JYY on 24-11-4.
//

#ifndef DATEUTIL_H
#define DATEUTIL_H

#include <QStringList>
#include "Plugin.h"

#define DATE_2_INT(d) \
    const QDate d0 = d; \
    (d0.year() << 12 | d0.month() << 8 | d0.day())

#define INT_2_DATE(i) QDate(i >> 12, (i >> 8) & 0xf, i & 0xff)

class CORE_API DateUtil {
public:
    static const QStringList WEEKDAYS_CN;
};

#endif //DATEUTIL_H
