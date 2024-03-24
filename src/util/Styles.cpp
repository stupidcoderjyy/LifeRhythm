//
// Created by stupid_coder_jyy on 2024/1/15.
//

#include "Styles.h"

#include <utility>

QMap<QString, Color*> Color::defaultColors{};

Color::Color(): WidgetData(), name(), rgbHex(), color() {
}

Color* Color::create(const QString& name, const QString& rgbHex) {
    auto* c = new Color(name, rgbHex);
    defaultColors.insert(name, c);
    return c;
}

Color::Color(QString name, const QString& rgbHex): WidgetData(), name(std::move(name)), rgbHex(rgbHex), color(rgbHex) {
}

QTextCharFormat Styles::FORMAT_DEFAULT;
QTextCharFormat Styles::FORMAT_ERROR;
QFont Styles::FONT_MAIN;

const Color* Styles::BLACK = Color::create("BLACK", "#141414");
const Color* Styles::GRAY_0 = Color::create("GRAY_0", "#282828");
const Color* Styles::GRAY_1 = Color::create("GRAY_1", "#363636");
const Color* Styles::GRAY_2 = Color::create("GRAY_2", "#444444");
const Color* Styles::GRAY_3 = Color::create("GRAY_3", "#525252");
const Color* Styles::GRAY_4 = Color::create("GRAY_4", "#606060");
const Color* Styles::CYAN_0 = Color::create("CYAN_0", "#204548");
const Color* Styles::CYAN_1 = Color::create("CYAN_1", "#3a6f74");
const Color* Styles::CYAN_2 = Color::create("CYAN_2", "#4d8f95");
const Color* Styles::BLUE_0 = Color::create("BLUE_0", "#2e436e");
const Color* Styles::BLUE_1 = Color::create("BLUE_1", "#3574f0");
const Color* Styles::RED = Color::create("RED", "#ff2b59");
const Color* Styles::GOLD = Color::create("GOLD", "#eecd66");
const Color* Styles::GREEN = Color::create("GREEN", "#48723c");
const Color* Styles::GRAY_TEXT_0 = Color::create("GRAY_TEXT_0", "#aeaeae");
const Color* Styles::GRAY_TEXT_1 = Color::create("GRAY_TEXT_1", "#C8C8C8");
const Color* Styles::CLEAR = Color::create("CLEAR", "#00000000");
const QString Styles::FF_ZH = "思源黑体 CN Medium";
const QString Styles::FF_EN = "JetBrains Mono Medium";
const int Styles::FS_MEDIUM = 10;

QFont fontMain() {
    QFont font;
    font.setFamilies({Styles::FF_EN, Styles::FF_ZH});
    font.setPointSize(Styles::FS_MEDIUM);
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
    FORMAT_DEFAULT = defaultFormat();
    FORMAT_ERROR = errorFormat();
}

QColor Styles::parseColor(const QString &str) {
    if (Color::defaultColors.contains(str)) {
        return Color::defaultColors.value(str)->color;
    }
    return {str};
}

