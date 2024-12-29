//
// Created by JYY on 24-11-7.
//

#ifndef INPUTTEXTLABEL_H
#define INPUTTEXTLABEL_H

#include <Namespaces.h>
#include "Label.h"
#include "LineEdit.h"

BEGIN_LR

class InputTextLabel : public Label {
private:
    LineEdit* edit;
    bool editing;
public:
    explicit InputTextLabel(QWidget *parent = Q_NULLPTR);
protected:
    virtual void initEditor();
    virtual void finishEditing();
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void stopEditing();
};

END_LR

#endif //INPUTTEXTLABEL_H
