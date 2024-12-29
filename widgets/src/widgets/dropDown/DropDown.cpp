//
// Created by JYY on 24-11-24.
//

#include "DropDown.h"
#include <FocusManager.h>
#include <NBT.h>
#include <Helpers.h>
#include <WidgetFactory.h>
#include <Error.h>
#include <QDebug>
#include <QLayout>
#include "Button.h"

USING_LR

DropDownMenu::DropDownMenu(QWidget *parent): Menu(parent) {
    setWindowModality(Qt::NonModal);
    setAttribute(Qt::WA_DeleteOnClose, false);
}

void DropDownMenu::mouseReleaseEvent(QMouseEvent *event) {
}

void DropDownMenu::focusOutEvent(QFocusEvent *event) {
    if (Helpers::isMouseHovered(this)) {
        setFocus();
    } else {
        emit sigSelectOption();
    }
}

DropDown::DropDown(QWidget *parent, bool i): FocusContainer(parent, i),
        menu(), pressLock(), menuOpen(), style(Jetbrains) {
}

void DropDown::onPostParsing(Handlers &handlers, NBT *nbt) {
    if (nbt->contains("style", Data::STRING)) {
        auto s = nbt->getString("style");
        Style style;
        if (s == "Jetbrains") {
            style = Jetbrains;
        } else if (s == "Button") {
            style = Button;
        } else {
            throw Error("invalid dropdown style: " + s);
        }
        handlers << [style](QWidget *w) {
            static_cast<DropDown*>(w)->setStyle(style);
        };
    }
}

void DropDown::onFinishedParsing(Handlers &handlers, NBT *widgetTag) {
    auto* menuTag = widgetTag->get("menu")->asCompound();
    if (menuTag) {
        auto* wfMenu = WidgetFactory::fromNbt("DropDownMenu", menuTag);
        auto* wfDropDown = WidgetFactory::factoryInParse();
        wfMenu->include(wfDropDown);
        wfMenu->overridePointerStorage(wfDropDown);
        try {
            wfMenu->parse();
        } catch (Error&) {
            delete wfMenu;
            throw;
        }
        handlers << [wfMenu](QWidget *w) {
            auto dd = static_cast<DropDown*>(w);
            dd->init0();
            wfMenu->apply(nullptr, dd->menu);
            delete wfMenu;
        };
    }
}

DropDown::~DropDown() {
    delete menu;
}

void DropDown::mousePressEvent(QMouseEvent *event) {
    if (!enabled) {
        return;
    }
    FocusContainer::mousePressEvent(event);
    if (!menuOpen && !pressLock) {
        FocusManager::mark(this);
        if (pressLock) {
            pressLock = false;
            return;
        }
        FocusManager::push(this);
        menu->activateWindow();
        auto p = Helpers::getGlobalPos(this);
        Helpers::moveWidget(menu, p.x(), p.y() + height() + 1);
        emit sigMenuOpening();
        menu->show();
        menuOpen = true;
    }
    if (!menuOpen && style == Button) {
        setStyleSheet(Button::QSS_HOVERED);
    }
}

void DropDown::mouseReleaseEvent(QMouseEvent *event) {
    pressLock = false;
}

void DropDown::focusOutEvent(QFocusEvent *event) {
    if (!FocusManager::checkPeek(this)) {
        FocusContainer::focusOutEvent(event);
        pressLock = false;
    }
}

void DropDown::enterEvent(QEnterEvent *event) {
    if (!enabled) {
        return;
    }
    if (!menuOpen && style == Button) {
        setStyleSheet(Button::QSS_HOVERED);
    }
}

void DropDown::leaveEvent(QEvent *event) {
    if (!enabled) {
        return;
    }
    if (!menuOpen && style == Button) {
        setStyleSheet(Button::QSS_NORMAL);
    }
}

QString DropDown::getNormalQss() {
    if (style == Jetbrains) {
        return FocusContainer::getNormalQss();
    }
    return Button::QSS_NORMAL;
}

QString DropDown::getFocusedQss() {
    if (style == Jetbrains) {
        return FocusContainer::getFocusedQss();
    }
    return Button::QSS_SELECTED;
}

void DropDown::initWidget() {
    FocusContainer::initWidget();
    if (!prepared) {
        init0();
    }
}

void DropDown::init0() {
    menu = new DropDownMenu();
    connect(menu, &DropDownMenu::sigSelectOption, this, [this] {
        emit menu->sigAboutToClose();
        menu->close();
    });
    connect(menu, &DropDownMenu::sigAboutToClose, this, [this] {
        pressLock = Helpers::isMouseHovered(this);
        menuOpen = false;
        if (style == Button) {
            setStyleSheet(Button::QSS_NORMAL);
        }
        FocusManager::popIfPeekMatch(this);
        emit sigMenuClosing();
    });
    connect(this, &DropDown::sigCloseMenu, menu, &DropDownMenu::sigSelectOption);
    prepared = true;
}
