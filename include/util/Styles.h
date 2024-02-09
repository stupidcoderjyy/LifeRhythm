//
// Created by stupid_coder_jyy on 2024/1/15.
//

#ifndef LIFERHYTHM_STYLES_H
#define LIFERHYTHM_STYLES_H

#include "QString"

class Styles {
public:
    const static QString BLACK;
    const static QString GRAY_0;
    const static QString GRAY_1;
    const static QString GRAY_2;
    const static QString GRAY_3;
    const static QString GRAY_4;
    const static QString GRAY_TEXT_0;
    const static QString MAIN_FONT;
    const static int FONT_SIZE_MEDIUM;
};

#define qss(k,d) QString(k).append(":").append(d).append(";")
#define qssbg(c) QString("background-color:").append(c).append(";")
#define qssborder(style,width,color) qss("border-style",style) + qss("border-width",width) + qss("border-color",color)

#endif //LIFERHYTHM_STYLES_H
