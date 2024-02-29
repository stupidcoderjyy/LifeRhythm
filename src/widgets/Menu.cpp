//
// Created by stupid_coder_jyy on 2024/2/28.
//

#include "Menu.h"
#include "Styles.h"
#include "WidgetUtil.h"

Menu::Menu(QWidget *parent): QWidget(parent), StandardWidget() {
    setWindowFlag(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::ClickFocus);
    setObjectName("menu");
    setStyleSheet(qss_t("menu", bg(Styles::GRAY_0) + bd("1px", "solid", Styles::GRAY_2) + brad("2px")));
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
