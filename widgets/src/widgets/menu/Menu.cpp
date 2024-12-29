//
// Created by stupid_coder_jyy on 2024/2/28.
//

#include "Menu.h"
#include <Helpers.h>
#include "Styles.h"

USING_LR

Menu::Menu(QWidget *parent, bool initInConstructor): QWidget(parent), StandardWidget(initInConstructor) {
    setWindowFlag(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::NoFocus);
}

void Menu::focusOutEvent(QFocusEvent *event) {
    if (!Helpers::isMouseHovered(this)) {
        emit sigAboutToClose();
        close();
    } else {
        setFocus();
    }
}

void Menu::showEvent(QShowEvent *event) {
    setFocus();
}

void Menu::initWidget() {
    if (objectName().isEmpty()) {
        setObjectName("menu");
    }
    setStyleSheet(qss_this(bg(Styles::GRAY_0->rgbHex) + bd("1px", "solid", Styles::GRAY_2->rgbHex) + brad("2px")));
}
