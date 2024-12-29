//
// Created by stupid_coder_jyy on 2024/2/13.
//

#ifndef STYLEPARSER_H
#define STYLEPARSER_H

#include <QTextCharFormat>
#include "NBT.h"
#include "Namespaces.h"

BEGIN_NP(lr::highlight)

class CORE_API StyleParser {
private:
    static QMap<QString, QTextCharFormat::UnderlineStyle> underlineStyles;
    QTextCharFormat* fmt;
    NBT* nbt;
public:
    static QTextCharFormat* parse(QTextCharFormat* fmt, NBT* nbt);
private:
    StyleParser(QTextCharFormat* fmt, NBT* nbt);
    void parseColor() const;
    void parseFont() const;
    void parseUnderline() const;
    static QMap<QString, QTextCharFormat::UnderlineStyle> initUnderlineStyles();
};


END_NP


#endif //STYLEPARSER_H
