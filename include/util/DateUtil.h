//
// Created by JYY on 24-11-4.
//

#ifndef DATEUTIL_H
#define DATEUTIL_H

#include <QStringList>
#include "Plugin.h"

#define DATE_2_INT(d) (d.year() << 12 | d.month() << 8 | d.day())
#define INT_2_DATE(i) QDate(i >> 12, (i >> 8) & 0xf, i & 0xff)

class CORE_API DateUtil {
public:
    static const QStringList WEEKDAYS_CN;
};

#endif //DATEUTIL_H
