//
// Created by stupid_coder_jyy on 2024/1/15.
//

#include "Styles.h"

QTextCharFormat Styles::FORMAT_DEFAULT;
QTextCharFormat Styles::FORMAT_ERROR;
QFont Styles::FONT_MAIN;

const QString Styles::BLACK = "#141414";
const QString Styles::GRAY_0 = "#282828";
const QString Styles::GRAY_1 = "#363636";
const QString Styles::GRAY_2 = "#444444";
const QString Styles::GRAY_3 = "#525252";
const QString Styles::GRAY_4 = "#606060";
const QString Styles::CYAN_DARK = "#204548";
const QString Styles::CYAN = "#3a6f74";
const QString Styles::CYAN_BRIGHT = "#4d8f95";
const QString Styles::RED = "#ff2b59";
const QString Styles::GOLD = "#eecd66";
const QString Styles::GREEN = "#48723c";
const QString Styles::GRAY_TEXT_0 = "#C8C8C8";
const QString Styles::CLEAR = "#00000000";
const QString Styles::FF_ZH = "Source Han Sans CN Medium";
const QString Styles::FF_EN = "JetBrains Mono Medium";
const int Styles::FS_MEDIUM = 12;

QMap<QString, QColor> initColors() {
    QMap<QString, QColor> res{};
    res.insert("BLACK", Styles::BLACK);
    res.insert("GRAY_0", Styles::GRAY_0);
    res.insert("GRAY_1", Styles::GRAY_1);
    res.insert("GRAY_2", Styles::GRAY_2);
    res.insert("GRAY_3", Styles::GRAY_3);
    res.insert("GRAY_4", Styles::GRAY_4);
    res.insert("GRAY_4", Styles::GRAY_4);
    res.insert("CYAN_DARK", Styles::CYAN_DARK);
    res.insert("CYAN", Styles::CYAN);
    res.insert("CYAN_BRIGHT", Styles::CYAN_BRIGHT);
    res.insert("RED", Styles::RED);
    res.insert("GOLD", Styles::GOLD);
    res.insert("GREEN", Styles::GREEN);
    res.insert("GRAY_TEXT_0", Styles::GRAY_TEXT_0);
    return res;
}

QFont fontMain() {
    QFont font;
    font.setFamilies({Styles::FF_EN, Styles::FF_ZH});
    font.setPointSize(Styles::FS_MEDIUM);
    return font;
}

QTextCharFormat defaultFormat() {
    QTextCharFormat fmt;
    fmt.setFont(fontMain());
    fmt.setForeground(QColor(Styles::GRAY_TEXT_0));
    return fmt;
}

QTextCharFormat errorFormat() {
    QTextCharFormat fmt;
    fmt.setFont(fontMain());
    fmt.setForeground(QColor(Styles::RED));
    return fmt;
}

QMap<QString, QColor> colors;

void Styles::initStyles() {
    colors = initColors();
    FONT_MAIN = fontMain();
    FORMAT_DEFAULT = defaultFormat();
    FORMAT_ERROR = errorFormat();
}

QColor Styles::parseColor(const QString &str) {
    return colors.value(str, QColor(str));
}

