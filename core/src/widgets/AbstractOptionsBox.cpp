//
// Created by stupid_coder_jyy on 2024/3/25.
//

#include "AbstractOptionsBox.h"
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

AbstractOptionsBox::AbstractOptionsBox(QWidget *parent): FocusContainer(parent), menu() {
    menuOpen = false;
    pressLock = false;
}

AbstractOptionsBox::~AbstractOptionsBox() {
    delete menu;
}

void AbstractOptionsBox::resizeEvent(QResizeEvent *event) {
    FocusContainer::resizeEvent(event);
    if (!menu) {
        menu = new OptionsMenu();
        initMenu(menu);
        connect(menu, &OptionsMenu::sigSelectOption, this, [this](){
            syncDataToWidget();
            emit menu->sigAboutToClose();
            menu->close();
        });
        connect(menu, &OptionsMenu::sigAboutToClose, this, &AbstractOptionsBox::onMenuClose);
    }
}

void AbstractOptionsBox::mousePressEvent(QMouseEvent *event) {
    FocusContainer::mousePressEvent(event);
    if (!menuOpen && !pressLock) {
        clickBox(true);
    }
}

void AbstractOptionsBox::mouseReleaseEvent(QMouseEvent *event) {
    pressLock = false;
}

void AbstractOptionsBox::focusOutEvent(QFocusEvent *event) {
    if (!FocusManager::checkPeek(this)) {
        FocusContainer::focusOutEvent(event);
        pressLock = false;
    }
}

void AbstractOptionsBox::clickBox(bool open) {
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

void AbstractOptionsBox::onMenuClose() {
    pressLock = isMouseHovered(this);
    menuOpen = false;
    FocusManager::popIfPeekMatch(this);
}