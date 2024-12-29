//
// Created by stupid_coder_jyy on 2024/2/27.
//

#ifndef FOCUSCONTAINER_H
#define FOCUSCONTAINER_H

#include "Widget.h"

BEGIN_LR

class FocusContainer : public Widget {
public:
    explicit FocusContainer(QWidget* parent = nullptr, bool initInConstructor = true);
protected:
    void focusOutEvent(QFocusEvent *event) override;
    virtual QString getNormalQss();
    virtual QString getFocusedQss();
protected:
    void initWidget() override;
    void mousePressEvent(QMouseEvent *event) override;
};

END_LR

#endif //FOCUSCONTAINER_H
