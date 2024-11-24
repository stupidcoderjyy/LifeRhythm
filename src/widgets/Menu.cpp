//
// Created by stupid_coder_jyy on 2024/2/28.
//

#include "Menu.h"
#include "Styles.h"
#include "WidgetUtil.h"

Menu::Menu(QWidget *parent, bool initInConstructor): QWidget(parent), StandardWidget(initInConstructor) {
    setWindowFlag(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("menu");
    setStyleSheet(qss_object("menu",
                    bg(Styles::GRAY_0->rgbHex) + bd("1px", "solid", Styles::GRAY_2->rgbHex) + brad("2px")));
}

void Menu::focusOutEvent(QFocusEvent *event) {
    if (!isMouseHovered(this)) {
        emit sigAboutToClose();
        close();
    } else {
        setFocus();
    }
}

void Menu::showEvent(QShowEvent *event) {
    setFocus();
}
