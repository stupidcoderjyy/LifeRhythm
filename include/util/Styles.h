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

class Styles {
public:
    const static QString BLACK;
    const static QString GRAY_0;
    const static QString GRAY_1;
    const static QString GRAY_2;
    const static QString GRAY_3;
    const static QString GRAY_4;
    const static QString CYAN_DARK;
    const static QString CYAN;
    const static QString CYAN_BRIGHT;
    const static QString RED;
    const static QString GOLD;
    const static QString GREEN;
    const static QString GRAY_TEXT_0;
    const static QString FF_ZH;
    const static QString FF_EN;
    const static QString CLEAR;
    const static int FS_MEDIUM;
    static QTextCharFormat FORMAT_DEFAULT;
    static QTextCharFormat FORMAT_ERROR;
    static QFont FONT_MAIN;

    static QColor parseColor(const QString& str);
    static void initStyles();
};

#define qss_t(name, qss) QString("#").append(name).append("{").append(qss).append("}")

#define qss(k,d) QString(k).append(":").append(d).append(";")
#define bg(c) QString("background-color:").append(c).append(";")
#define bd_t(a,b,c) QString("border-top:").append(a).append(" ").append(b).append(" ").append(c).append(";")
#define bd_b(a,b,c) QString("border-bottom:").append(a).append(" ").append(b).append(" ").append(c).append(";")
#define bd_l(a,b,c) QString("border-left:").append(a).append(" ").append(b).append(" ").append(c).append(";")
#define bd_r(a,b,c) QString("border-right:").append(a).append(" ").append(b).append(" ").append(c).append(";")
#define bd(a,b,c) QString("border:").append(a).append(" ").append(b).append(" ").append(c).append(";")
#define PAINT_QSS \
    protected: void paintEvent(QPaintEvent *event) override{\
        Q_UNUSED(event);\
        QStyleOption opt;\
        opt.init(this);\
        QPainter p(this);\
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);\
    };

#endif //LIFERHYTHM_STYLES_H
