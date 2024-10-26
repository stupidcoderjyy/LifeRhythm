//
// Created by stupid_coder_jyy on 2024/2/28.
//

#ifndef LIFERHYTHM_LINEEDIT_H
#define LIFERHYTHM_LINEEDIT_H

#include <QLineEdit>
#include "StandardWidget.h"

class CORE_API LineEdit : public QLineEdit, public StandardWidget {
public:
    explicit LineEdit(QWidget* parent = nullptr);
    void onPreParsing(Handlers &handlers, NBT *widgetTag) override;
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void onStateRespondersParsing(Handlers &responders, NBT *stateTag) override;
protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void init();
};


#endif //LIFERHYTHM_LINEEDIT_H
