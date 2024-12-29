//
// Created by JYY on 24-12-13.
//

#include "ButtonSwitchView.h"
#include <Namespaces.h>

USING_NP(lr::calendar)

ButtonSwitchView::ButtonSwitchView(QWidget *parent, bool iic): Button(parent, iic) {
}

void ButtonSwitchView::handleButtonActivate(QMouseEvent *ev) {
    if (!selected) {
        Button::handleButtonActivate(ev);
    }
}
