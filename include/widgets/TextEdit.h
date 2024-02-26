//
// Created by stupid_coder_jyy on 2024/2/14.
//

#ifndef LIFERHYTHM_TEXTEDIT_H
#define LIFERHYTHM_TEXTEDIT_H

#include <QTextEdit>
#include "ScrollBar.h"

class TextEdit : public QTextEdit{
public:
    explicit TextEdit(QWidget* parent);
SCROLL_CLAZZ_DEF
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
};


#endif //LIFERHYTHM_TEXTEDIT_H
