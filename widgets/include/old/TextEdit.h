//
// Created by stupid_coder_jyy on 2024/2/14.
//

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include "ScrollBar.h"
#include "StandardWidget.h"

BEGIN_LR

class CORE_API TextEdit : public QTextEdit, public StandardWidget {
protected:
    ScrollBar *vBar{};
    ScrollBar *hBar{};
public:
    explicit TextEdit(QWidget* parent, bool initInConstructor);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void onStateRespondersParsing(Handlers &responders, NBT *stateTag) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
};

END_LR

#endif //TEXTEDIT_H
