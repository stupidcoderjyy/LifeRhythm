//
// Created by stupid_coder_jyy on 2024/2/28.
//

#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include "StandardWidget.h"

BEGIN_LR

class CORE_API LineEdit : public QLineEdit, public StandardWidget {
public:
    explicit LineEdit(QWidget* parent = nullptr, bool initInConstructor = true);
    void onPreParsing(Handlers &handlers, NBT *widgetTag) override;
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void onStateRespondersParsing(Handlers &responders, NBT *stateTag) override;
protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;
    void initWidget() override;
};

END_LR

#endif //LINEEDIT_H