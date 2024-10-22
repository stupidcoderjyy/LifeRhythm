//
// Created by stupid_coder_jyy on 2024/2/27.
//

#ifndef LIFERHYTHM_FOCUSCONTAINER_H
#define LIFERHYTHM_FOCUSCONTAINER_H

#include "Widget.h"

/*
    一个容器组件。在获得焦点的状态下高亮边框，
    失去焦点时，显示一个浅边框
*/
class CORE_API FocusContainer : public Widget {
public:
    explicit FocusContainer(QWidget* parent = nullptr);
protected:
    void focusOutEvent(QFocusEvent *event) override;
    virtual QString getNormalQss();
    virtual QString getFocusedQss();
    void resizeEvent(QResizeEvent *event) override;
protected:
    void mousePressEvent(QMouseEvent *event) override;
};


#endif //LIFERHYTHM_FOCUSCONTAINER_H
