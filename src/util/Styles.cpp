//
// Created by stupid_coder_jyy on 2024/1/15.
//

#include "Styles.h"

QMap<QString, QColor> Styles::colors;
QTextCharFormat Styles::FORMAT_DEFAULT;
QTextCharFormat Styles::FORMAT_ERROR;
QFont Styles::FONT_MAIN;

const QString Styles::BLACK = "#141414";
const QString Styles::GRAY_0 = "#282828";
const QString Styles::GRAY_1 = "#363636";
const QString Styles::GRAY_2 = "#444444";
const QString Styles::GRAY_3 = "#525252";
const QString Styles::GRAY_4 = "#606060";
const QString Styles::CYAN_0 = "#204548";
const QString Styles::CYAN_1 = "#3a6f74";
const QString Styles::CYAN_2 = "#4d8f95";
const QString Styles::BLUE_0 = "#2e436e";
const QString Styles::BLUE_1 = "#3574f0";
const QString Styles::RED = "#ff2b59";
const QString Styles::GOLD = "#eecd66";
const QString Styles::GREEN = "#48723c";
const QString Styles::GRAY_TEXT_0 = "#aeaeae";
const QString Styles::GRAY_TEXT_1 = "#C8C8C8";
const QString Styles::CLEAR = "#00000000";
const QString Styles::FF_ZH = "思源黑体 CN Medium";
const QString Styles::FF_EN = "JetBrains Mono Medium";
const int Styles::FS_MEDIUM = 10;

#define regColor(c) res.insert(#c, Styles::c)

QMap<QString, QColor> initColors() {
    QMap<QString, QColor> res{};
    regColor(GRAY_0);
    regColor(GRAY_1);
    regColor(GRAY_2);
    regColor(GRAY_3);
    regColor(GRAY_4);
    regColor(BLACK);
    regColor(CYAN_0);
    regColor(CYAN_1);
    regColor(CYAN_2);
    regColor(BLUE_0);
    regColor(BLUE_1);
    regColor(RED);
    regColor(GOLD);
    regColor(GREEN);
    regColor(GRAY_TEXT_0);
    regColor(GRAY_TEXT_1);
    return res;
}

#undef regColor

QFont fontMain() {
    QFont font;
    font.setFamilies({Styles::FF_EN, Styles::FF_ZH});
    font.setPointSize(Styles::FS_MEDIUM);
    return font;
}

QTextCharFormat defaultFormat() {
    QTextCharFormat fmt;
    fmt.setFont(fontMain());
    fmt.setForeground(QColor(Styles::GRAY_TEXT_1));
    return fmt;
}

QTextCharFormat errorFormat() {
    QTextCharFormat fmt;
    fmt.setFont(fontMain());
    fmt.setForeground(QColor(Styles::RED));
    return fmt;
}

void Styles::initStyles() {
    colors = initColors();
    FONT_MAIN = fontMain();
    FORMAT_DEFAULT = defaultFormat();
    FORMAT_ERROR = errorFormat();
}

QColor Styles::parseColor(const QString &str) {
    return colors.value(str, QColor(str));
}

