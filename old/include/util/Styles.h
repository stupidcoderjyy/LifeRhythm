//
// Created by stupid_coder_jyy on 2024/1/15.
//

#ifndef LIFERHYTHM_STYLES_H
#define LIFERHYTHM_STYLES_H

#include <QStyleOption>
#include <QTextCharFormat>
#include "WidgetData.h"

class CORE_API Color final : public WidgetData {
public:
    static QMap<QString, Color*> defaultColors;
    QString name;
    QString rgbHex;
    QColor color;
public:
    Color();
    explicit Color(const QString& rgbHex);
    Color(QString name, const QString& rgbHex);
    static Color* createDefault(const QString& name, const QString& rgbHex);
};

class CORE_API FontType {
private:
    QStringList ffMedium;
    QStringList ffBold;
public:
    FontType(QStringList ffMedium, QStringList ffBold);
    FontType(const FontType& o) = default;
    inline const QStringList& getFFMedium();
    inline const QStringList& getFFBold();
};

inline const QStringList& FontType::getFFMedium() {
    return ffMedium;
}
inline const QStringList& FontType::getFFBold() {
    return ffBold;
}

class CORE_API FontBuilder {
private:
    QFont font;
    FontType type;
public:
    FontBuilder();
    explicit FontBuilder(const FontType& type);
    FontBuilder(const FontType& type, const QFont& f);
    FontBuilder(const FontBuilder& o) = default;
    FontBuilder& setSmallSize();
    FontBuilder& setMediumSize();
    FontBuilder& setLargeSize();
    FontBuilder& setBoldWeight();
    FontBuilder& setNormalWeight();
    int getTextHeight() const;
    int getTextWidth(const QString& text) const;
    const QFont& get();
};

class CORE_API Styles {
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
    const static Color* GREEN_0;
    const static Color* GREEN_1;
    const static Color* GREEN_2;
    const static Color* GRAY_TEXT_0;
    const static Color* GRAY_TEXT_1;
    const static Color* CLEAR;

    const static FontType FONT_TYPE_MAIN;
    const static int FS_SMALL;
    const static int FS_MEDIUM;
    const static int FS_LARGE;
    static QTextCharFormat FORMAT_DEFAULT;
    static QTextCharFormat FORMAT_ERROR;
    static QFont FONT_SMALL;
    static QFont FONT_MAIN;
    static QFont FONT_LARGE;

    static QColor parseColor(const QString& str);
    static void initStyles();
};

#define qss_object(name, qss) QString("#").append(name).append("{").append(qss).append("}")
#define qss_this(qss) QString("#").append(objectName()).append("{").append(qss).append("}")
#define qss_target(target, qss) QString(target).append("{").append(qss).append("}")

#define qss(k,d) QString(k).append(":").append(d).append(";")
#define bg(c) QString("background-color:").append(c).append(";")
#define clr(c) QString("color:").append(c).append(";")
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
