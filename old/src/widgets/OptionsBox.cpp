//
// Created by stupid_coder_jyy on 2024/3/25.
//

#include "OptionsBox.h"
#include "WidgetUtil.h"
#include "FocusManager.h"
#include <QDebug>

OptionsMenu::OptionsMenu(QWidget *parent): Menu(parent) {
    setWindowModality(Qt::NonModal);
    setAttribute(Qt::WA_DeleteOnClose, false);
}

void OptionsMenu::mouseReleaseEvent(QMouseEvent *event) {
}

void OptionsMenu::focusOutEvent(QFocusEvent *event) {
    if (isMouseHovered(this)) {
        setFocus();
    } else {
        emit sigSelectOption();
    }
}

OptionsBox::OptionsBox(QWidget *parent, bool initInConstructor): FocusContainer(parent, initInConstructor), menu() {
    menuOpen = false;
    pressLock = false;
}

OptionsBox::~OptionsBox() {
    delete menu;
}

void OptionsBox::mousePressEvent(QMouseEvent *event) {
    FocusContainer::mousePressEvent(event);
    if (!menuOpen && !pressLock) {
        clickBox(true);
    }
}

void OptionsBox::mouseReleaseEvent(QMouseEvent *event) {
    pressLock = false;
}

void OptionsBox::focusOutEvent(QFocusEvent *event) {
    if (!FocusManager::checkPeek(this)) {
        FocusContainer::focusOutEvent(event);
        pressLock = false;
    }
}

void OptionsBox::clickBox(bool open) {
    FocusManager::mark(this);
    if (open) {
        if (pressLock) {
            pressLock = false;
            return;
        }
        FocusManager::push(this);
        menu->activateWindow();
        auto p = getGlobalPos(this);
        moveWidget(menu, p.x(), p.y() + height() + 1);
        menu->show();
        menuOpen = true;
    } else {
        emit menu->sigSelectOption();   //close
    }
}

void OptionsBox::initMenu(OptionsMenu *menu) {
}

void OptionsBox::onMenuClose() {
    pressLock = isMouseHovered(this);
    menuOpen = false;
    FocusManager::popIfPeekMatch(this);
}

void OptionsBox::initWidget() {
    FocusContainer::initWidget();
    menu = new OptionsMenu();
    initMenu(menu);
    connect(menu, &OptionsMenu::sigSelectOption, this, [this] {
        syncDataToWidget();
        emit menu->sigAboutToClose();
        menu->close();
    });
    connect(menu, &OptionsMenu::sigAboutToClose, this, &OptionsBox::onMenuClose);
}
