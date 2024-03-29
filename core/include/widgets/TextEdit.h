//
// Created by stupid_coder_jyy on 2024/2/14.
//

#ifndef LIFERHYTHM_TEXTEDIT_H
#define LIFERHYTHM_TEXTEDIT_H

#include <QTextEdit>
#include "ScrollBar.h"
#include "StandardWidget.h"

class TextEdit : public QTextEdit, public StandardWidget {
protected:
    ScrollBar *vBar{};
    ScrollBar *hBar{};
public:
    explicit TextEdit(QWidget* parent);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void onStateRespondersParsing(Handlers &responders, NBT *stateTag) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
};


#endif //LIFERHYTHM_TEXTEDIT_H
