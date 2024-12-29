//
// Created by stupid_coder_jyy on 2024/2/13.
//

#include "StyleParser.h"
#include "Styles.h"

USING_NP(lr::highlight)

QMap<QString, QTextCharFormat::UnderlineStyle> StyleParser::underlineStyles = initUnderlineStyles();

QTextCharFormat *StyleParser::parse(QTextCharFormat* fmt, NBT *nbt) {
    StyleParser parser(fmt, nbt);
    parser.parseColor();
    parser.parseFont();
    parser.parseUnderline();
    return parser.fmt;
}

StyleParser::StyleParser(QTextCharFormat *fmt, NBT *nbt):fmt(fmt),nbt(nbt){
}

void StyleParser::parseColor() const {
    if (nbt->contains("fg", Data::STRING)) {
        fmt->setForeground(Styles::parseColor(nbt->getString("fg")));
    }
    if (nbt->contains("bg", Data::STRING)) {
        fmt->setBackground(Styles::parseColor(nbt->getString("bg")));
    }
}

void StyleParser::parseFont() const {
    auto* ffTag = nbt->get("ff");
    if (ffTag) {
        if (ffTag->type == Data::STRING) {
            fmt->setFontFamilies({ffTag->asString()->get()});
        } else if (ffTag->type == Data::ARR) {
            QStringList list;
            ffTag->asArray()->fillString(list);
            fmt->setFontFamilies(list);
        }
    }
    if (nbt->contains("fs", Data::INT)) {
        fmt->setFontPointSize(nbt->getInt("fs"));
    }
    fmt->setFontWeight(nbt->getBool("bold") ? QFont::Bold : QFont::Normal);
}

void StyleParser::parseUnderline() const {
    fmt->setUnderlineStyle(underlineStyles.value(
            nbt->getString("ul"),QTextCharFormat::NoUnderline));
    fmt->setUnderlineColor(Styles::parseColor(nbt->getString("ulc", Styles::GRAY_TEXT_0->rgbHex)));
}

#define reg(k) map.insert(#k, QTextCharFormat::k)

QMap<QString, QTextCharFormat::UnderlineStyle> StyleParser::initUnderlineStyles() {
    QMap<QString, QTextCharFormat::UnderlineStyle> map;
    reg(NoUnderline);
    reg(SingleUnderline);
    reg(DashUnderline);
    reg(DashDotLine);
    reg(DashDotDotLine);
    reg(WaveUnderline);
    reg(SpellCheckUnderline);
    return map;
}
