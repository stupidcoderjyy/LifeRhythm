//
// Created by stupid_coder_jyy on 2024/2/13.
//

#ifndef LIFERHYTHM_STYLEPARSER_H
#define LIFERHYTHM_STYLEPARSER_H

#include <QTextCharFormat>
#include "NBT.h"

namespace highlight {

class CORE_API StyleParser {
private:
    static QMap<QString, QTextCharFormat::UnderlineStyle> underlineStyles;
    QTextCharFormat* fmt;
    NBT* nbt;
public:
    static QTextCharFormat* parse(QTextCharFormat* fmt, NBT* nbt);
private:
    StyleParser(QTextCharFormat* fmt, NBT* nbt);
    void parseColor();
    void parseFont();
    void parseUnderline();
    static QMap<QString, QTextCharFormat::UnderlineStyle> initUnderlineStyles();
};

}


#endif //LIFERHYTHM_STYLEPARSER_H
