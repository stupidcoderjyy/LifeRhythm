//
// Created by stupid_coder_jyy on 2024/2/28.
//

#ifndef LIFERHYTHM_LINEEDIT_H
#define LIFERHYTHM_LINEEDIT_H

#include <QLineEdit>
#include "StandardWidget.h"

class LineEdit : public QLineEdit, public StandardWidget {
public:
    explicit LineEdit(QWidget* parent = nullptr);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void onStateRespondersParsing(Handlers &responders, NBT *stateTag) override;
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
};


#endif //LIFERHYTHM_LINEEDIT_H
