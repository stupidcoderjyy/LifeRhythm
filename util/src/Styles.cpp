//
// Created by stupid_coder_jyy on 2024/1/15.
//

#include "Styles.h"
#include <utility>

USING_LR

QMap<QString, Color*> Color::defaultColors{};

Color::Color() {
}

Color::Color(const QString &rgbHex): rgbHex(rgbHex), color(rgbHex) {
}

Color::Color(QString name, const QString& rgbHex): name(std::move(name)), rgbHex(rgbHex), color(rgbHex) {
}

Color* Color::createDefault(const QString& name, const QString& rgbHex) {
    auto* c = new Color(name, rgbHex);
    defaultColors.insert(name, c);
    return c;
}

FontType::FontType(QStringList f1, QStringList f2): ffMedium(std::move(f1)), ffBold(std::move(f2)) {
}

FontBuilder::FontBuilder(): type({}, {}) {
}

FontBuilder::FontBuilder(const FontType& type): type(type) {
}

FontBuilder::FontBuilder(const FontType &type, const QFont& f): font(f), type(type) {
}

FontBuilder &FontBuilder::setSmallSize() {
    font.setPointSize(Styles::FS_SMALL);
    return *this;
}

FontBuilder &FontBuilder::setMediumSize() {
    font.setPointSize(Styles::FS_MEDIUM);
    return *this;
}

FontBuilder &FontBuilder::setLargeSize() {
    font.setPointSize(Styles::FS_LARGE);
    return *this;
}

FontBuilder &FontBuilder::setBoldWeight() {
    if (type.getFFBold().empty()) {
        return *this;
    }
    if (type.getFFBold().size() > 1) {
        font.setFamilies(type.getFFBold());
    } else {
        font.setFamily(type.getFFBold()[0]);
    }
    return *this;
}

FontBuilder &FontBuilder::setNormalWeight() {
    if (type.getFFMedium().empty()) {
        return *this;
    }
    if (type.getFFMedium().size() > 1) {
        font.setFamilies(type.getFFMedium());
    } else {
        font.setFamily(type.getFFMedium()[0]);
    }
    return *this;
}

int FontBuilder::getTextHeight() const {
    QFontMetrics metrics(font);
    return metrics.height();
}

int FontBuilder::getTextWidth(const QString &text) const {
    QFontMetrics metrics(font);
    return metrics.horizontalAdvance(text);
}

const QFont& FontBuilder::get() {
    font.setStyleStrategy(QFont::PreferAntialias);
    return font;
}

QTextCharFormat Styles::FORMAT_DEFAULT;
QTextCharFormat Styles::FORMAT_ERROR;
QFont Styles::FONT_SMALL;
QFont Styles::FONT_MAIN;
QFont Styles::FONT_LARGE;

const Color* Styles::BLACK = Color::createDefault("BLACK", "#141414");
const Color* Styles::GRAY_0 = Color::createDefault("GRAY_0", "#282828");
const Color* Styles::GRAY_1 = Color::createDefault("GRAY_1", "#363636");
const Color* Styles::GRAY_2 = Color::createDefault("GRAY_2", "#444444");
const Color* Styles::GRAY_3 = Color::createDefault("GRAY_3", "#525252");
const Color* Styles::GRAY_4 = Color::createDefault("GRAY_4", "#606060");
const Color* Styles::CYAN_0 = Color::createDefault("CYAN_0", "#204548");
const Color* Styles::CYAN_1 = Color::createDefault("CYAN_1", "#3a6f74");
const Color* Styles::CYAN_2 = Color::createDefault("CYAN_2", "#4d8f95");
const Color* Styles::BLUE_0 = Color::createDefault("BLUE_0", "#2e436e");
const Color* Styles::BLUE_1 = Color::createDefault("BLUE_1", "#3574f0");
const Color* Styles::RED = Color::createDefault("RED", "#ff2b59");
const Color* Styles::GOLD = Color::createDefault("GOLD", "#eecd66");
const Color* Styles::GREEN_0 = Color::createDefault("GREEN_0", "#313c2e");
const Color* Styles::GREEN_1 = Color::createDefault("GREEN_1", "#48723c");
const Color* Styles::GREEN_2 = Color::createDefault("GREEN_1", "#609451");
const Color* Styles::GRAY_TEXT_0 = Color::createDefault("GRAY_TEXT_0", "#aeaeae");
const Color* Styles::GRAY_TEXT_1 = Color::createDefault("GRAY_TEXT_1", "#C8C8C8");
const Color* Styles::CLEAR = Color::createDefault("CLEAR", "#00000000");
const FontType Styles::FONT_TYPE_MAIN = FontType(
        {"JetBrains Mono Medium", "思源黑体 CN Medium"},
        {"JetBrains Mono Bold", "思源黑体 CN Bold"});
const int Styles::FS_SMALL = 10;
const int Styles::FS_MEDIUM = 12;
const int Styles::FS_LARGE = 15;

QTextCharFormat defaultFormat() {
    QTextCharFormat fmt;
    fmt.setFont(Styles::FONT_MAIN);
    fmt.setForeground(Styles::GRAY_TEXT_1->color);
    return fmt;
}

QTextCharFormat errorFormat() {
    QTextCharFormat fmt;
    fmt.setFont(Styles::FONT_MAIN);
    fmt.setForeground(Styles::RED->color);
    return fmt;
}

void Styles::initStyles() {
    auto fb = FontBuilder(FONT_TYPE_MAIN).setNormalWeight();
    FONT_MAIN = fb.setMediumSize().get();
    FONT_SMALL = fb.setSmallSize().get();
    FONT_LARGE = fb.setLargeSize().get();
    FORMAT_DEFAULT = defaultFormat();
    FORMAT_ERROR = errorFormat();
}

QColor Styles::parseColor(const QString &str) {
    if (Color::defaultColors.contains(str)) {
        return Color::defaultColors.value(str)->color;
    }
    return {str};
}

