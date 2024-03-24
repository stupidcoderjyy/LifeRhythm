//
// Created by stupid_coder_jyy on 2024/1/15.
//

#ifndef LIFERHYTHM_STYLES_H
#define LIFERHYTHM_STYLES_H

#include <QString>
#include <QStyle>
#include <QPainter>
#include <QStyleOption>
#include <QTextCharFormat>
#include "WidgetData.h"

class Color : public WidgetData {
public:
    static QMap<QString, Color*> defaultColors;
    QString name;
    QString rgbHex;
    QColor color;
public:
    Color();
    static Color* create(const QString& name, const QString& rgbHex);
private:
    Color(QString name, const QString& rgbHex);
};

class Styles {
public:
    const static Color* BLACK;
    const static Color* GRAY_0;
    const static Color* GRAY_1;
    const static Color* GRAY_2;
    const static Color* GRAY_3;
    const static Color* GRAY_4;
    const static Color* CYAN_0;
    const static Color* CYAN_1;
    const static Color* CYAN_2;
    const static Color* BLUE_0;
    const static Color* BLUE_1;
    const static Color* RED;
    const static Color* GOLD;
    const static Color* GREEN;
    const static Color* GRAY_TEXT_0;
    const static Color* GRAY_TEXT_1;
    const static Color* CLEAR;

    const static QString FF_ZH;
    const static QString FF_EN;
    const static int FS_MEDIUM;
    static QTextCharFormat FORMAT_DEFAULT;
    static QTextCharFormat FORMAT_ERROR;
    static QFont FONT_MAIN;

    static QColor parseColor(const QString& str);
    static void initStyles();
};

#define qss_target(name, qss) QString("#").append(name).append("{").append(qss).append("}")
#define qss_this(qss) QString("#").append(objectName()).append("{").append(qss).append("}")

#define qss(k,d) QString(k).append(":").append(d).append(";")
#define bg(c) QString("background-color:").append(c).append(";")
#define bd_t(width,style,color) QString("border-top:").append(width).append(" ").append(style).append(" ").append(color).append(";")
#define bd_b(width,style,color) QString("border-bottom:").append(width).append(" ").append(style).append(" ").append(color).append(";")
#define bd_l(width,style,color) QString("border-left:").append(width).append(" ").append(style).append(" ").append(color).append(";")
#define bd_r(width,style,color) QString("border-right:").append(width).append(" ").append(style).append(" ").append(color).append(";")
#define bd(width,style,color) QString("border:").append(width).append(" ").append(style).append(" ").append(color).append(";")
#define brad(width) qss("border-radius", width)
#define PAINT_QSS \
    protected: void paintEvent(QPaintEvent *event) override{\
        Q_UNUSED(event);\
        QStyleOption opt;\
        opt.init(this);\
        QPainter p(this);\
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);\
    };

#endif //LIFERHYTHM_STYLES_H
