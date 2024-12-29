//
// Created by JYY on 24-12-24.
//

#ifndef HELPERS_H
#define HELPERS_H
#include "Namespaces.h"
#include <QWidget>
#include "Plugin.h"

BEGIN_LR

class CORE_API Helpers {
public:
    static const QStringList WEEKDAYS_CN;
    static int **allocateArray(int rows, int columns);
    static int *allocateArray(int size);
    static void freeArray(int **p, int rows);
    static bool isHovered(QWidget* widget, QMouseEvent* evt);
    static bool isHovered(QWidget* widget, const QPoint& pos);
    static bool isMouseHovered(QWidget* widget);
    static QPoint getGlobalPos(QWidget* widget);
    static void moveWidget(QWidget* widget, const QPoint& pos);
    static void moveWidget(QWidget* widget, int x, int y);
    static void switchSingleLayoutWidget(QLayout* layout, QWidget* pre, QWidget* cur);
    static void fillRoundedRect(QPainter& painter, const QRect& rect, int radius, const QBrush& color);
};

END_LR

#define DATE_2_INT(d) \
    const QDate d0 = d; \
    (d0.year() << 12 | d0.month() << 8 | d0.day())

#define INT_2_DATE(i) QDate(i >> 12, (i >> 8) & 0xf, i & 0xff)

#define DELETE_MAP(qMap) \
    for (auto it = (qMap).begin() ; it != (qMap).end() ; it++) {\
        delete it.value();\
    }\
    (qMap).clear();

#define DELETE_MAP_P(qMap) \
    for (auto it = (qMap)->begin() ; it != (qMap)->end() ; it++) {\
        delete it.value();\
    }\
    (qMap)->clear();

#define DELETE_LIST_P(pList) \
    for (auto* d : *(pList)) {\
        delete d;\
    }\
    (pList)->clear();

#define DELETE_LIST(list) \
    for (auto* d : list) {\
        delete d;\
    }\
    list.clear();

#define DELETE_NESTED_LIST(list) \
    for (auto* child : list) {\
        DELETE_LIST_P(child);\
        delete child;\
    }\
    list.clear();

#endif //HELPERS_H
