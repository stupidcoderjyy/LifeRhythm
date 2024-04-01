//
// Created by stupid_coder_jyy on 2024/1/15.
//

#include "Styles.h"

#include <utility>

QMap<QString, Color*> Color::defaultColors{};

Color::Color(): WidgetData(), name(), rgbHex(), color() {
}

Color::Color(const QString &rgbHex): name(), rgbHex(rgbHex), color(rgbHex) {
}

Color::Color(QString name, const QString& rgbHex): WidgetData(), name(std::move(name)), rgbHex(rgbHex), color(rgbHex) {
}

Color* Color::createDefault(const QString& name, const QString& rgbHex) {
    auto* c = new Color(name, rgbHex);
    defaultColors.insert(name, c);
    return c;
}

QTextCharFormat Styles::FORMAT_DEFAULT;
QTextCharFormat Styles::FORMAT_ERROR;
QFont Styles::FONT_MAIN;
QFont Styles::FONT_SMALL;

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
const Color* Styles::GREEN = Color::createDefault("GREEN", "#48723c");
const Color* Styles::GRAY_TEXT_0 = Color::createDefault("GRAY_TEXT_0", "#aeaeae");
const Color* Styles::GRAY_TEXT_1 = Color::createDefault("GRAY_TEXT_1", "#C8C8C8");
const Color* Styles::CLEAR = Color::createDefault("CLEAR", "#00000000");
const QString Styles::FF_ZH = "思源黑体 CN Medium";
const QString Styles::FF_EN = "JetBrains Mono Medium";
const int Styles::FS_MEDIUM = 10;
const int Styles::FS_SMALL = 8;

QFont fontMain() {
    QFont font;
    font.setFamilies({Styles::FF_EN, Styles::FF_ZH});
    font.setPointSize(Styles::FS_MEDIUM);
    return font;
}

QFont fontSmall() {
    QFont font;
    font.setFamilies({Styles::FF_EN, Styles::FF_ZH});
    font.setPointSize(Styles::FS_SMALL);
    return font;
}

QTextCharFormat defaultFormat() {
    QTextCharFormat fmt;
    fmt.setFont(fontMain());
    fmt.setForeground(Styles::GRAY_TEXT_1->color);
    return fmt;
}

QTextCharFormat errorFormat() {
    QTextCharFormat fmt;
    fmt.setFont(fontMain());
    fmt.setForeground(Styles::RED->color);
    return fmt;
}

void Styles::initStyles() {
    FONT_MAIN = fontMain();
    FONT_SMALL = fontSmall();
    FORMAT_DEFAULT = defaultFormat();
    FORMAT_ERROR = errorFormat();
}

QColor Styles::parseColor(const QString &str) {
    if (Color::defaultColors.contains(str)) {
        return Color::defaultColors.value(str)->color;
    }
    return {str};
}

