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
class FocusContainer : public Widget {
private:
    static FocusContainer* activated;
public:
    explicit FocusContainer(QWidget* parent = nullptr);
    void setContainerState(int focused = true);
    ~FocusContainer() override;
protected:
    virtual QString getNormalQss();
    virtual QString getFocusedQss();
protected:
    void showEvent(QShowEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};


#endif //LIFERHYTHM_FOCUSCONTAINER_H
