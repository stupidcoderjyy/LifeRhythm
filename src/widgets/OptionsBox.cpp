//
// Created by stupid_coder_jyy on 2024/2/27.
//

#include "OptionsBox.h"
#include "QHBoxLayout"
#include "RcManagers.h"
#include "WidgetUtil.h"
#include "FocusManager.h"
#include <QApplication>
#include <QMouseEvent>

OptionsMenu::OptionsMenu(QWidget *parent): Menu(parent) {
    setWindowModality(Qt::NonModal);
}

void OptionsMenu::mouseReleaseEvent(QMouseEvent *event) {
}

DisplayOptionsButton::DisplayOptionsButton(QWidget *parent): ArrowButton(parent) {
    setFixedSize(20, 20);
    setRatio(0.8, 0.4);
    setScale(0.6);
}

void DisplayOptionsButton::mousePressEvent(QMouseEvent *evt) {
    ArrowButton::mousePressEvent(evt);
    evt->ignore();
}

OptionsBoxLineEdit::OptionsBoxLineEdit(QWidget *parent): QLineEdit(parent) {
    setFrame(false);
    setFocusPolicy(Qt::ClickFocus);
    setContextMenuPolicy(Qt::NoContextMenu);
    setFont(Styles::FONT_MAIN);
    setObjectName("le");
    setStyleSheet(qss_this(bg(Styles::CLEAR) + qss("color", Styles::GRAY_TEXT_0)));
}

void OptionsBoxLineEdit::mousePressEvent(QMouseEvent *e) {
    QLineEdit::mousePressEvent(e);
    e->ignore();
    QApplication::setCursorFlashTime(0);
}

void OptionsBoxLineEdit::mouseReleaseEvent(QMouseEvent *e) {
    QLineEdit::mouseReleaseEvent(e);
    QApplication::setCursorFlashTime(1060);
}

OptionsBox::OptionsBox(QWidget *parent): FocusContainer(parent), menu(), pressLock() {
    auto* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(5, 0, 8, 0);
    setLayout(layout);
    optionEditor = new LineEdit(this);
    optionEditor->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    buttonOpenMenu = new DisplayOptionsButton(this);
    layout->addWidget(optionEditor);
    layout->addWidget(buttonOpenMenu);
    setMinimumWidth(200);
    setFixedHeight(40);
    setContentsMargins(0, 0, 0, 0);
    connect(buttonOpenMenu, &ArrowButton::sigActivated, this, &OptionsBox::handleButtonClick);
}

void OptionsBox::fillOption(WidgetData *data) {
}

void OptionsBox::initMenu(OptionsMenu* m) {
}

void OptionsBox::resizeEvent(QResizeEvent *event) {
    if (!menu) {
        menu = new OptionsMenu();
        initMenu(menu);
        connect(menu, &OptionsMenu::sigSelectOption, this, [this](WidgetData* data){
            fillOption(data);
            emit menu->sigAboutToClose();
            menu->close();
        });
        connect(menu, &OptionsMenu::sigAboutToClose, this, &OptionsBox::onMenuClose);
    }
}

OptionsBox::~OptionsBox() {
    delete menu;
}

void OptionsBox::mouseReleaseEvent(QMouseEvent *event) {
    pressLock = false;
}

void OptionsBox::focusOutEvent(QFocusEvent *event) {
    FocusContainer::focusOutEvent(event);
    pressLock = false;
}

void OptionsBox::handleButtonClick(bool pressed) {
    FocusManager::markFocus(this);
    if (pressed) {
        if (pressLock) {
            pressLock = false;
            buttonOpenMenu->setPressed(false);
            return;
        }
        menu->setMinimumWidth(width());
        menu->activateWindow();
        auto p = getGlobalPos(this);
        moveWidget(menu, p.x(), p.y() + height() + 1);
        menu->show();
    } else {
        menu->close();
    }
}

void OptionsBox::onMenuClose() {
    buttonOpenMenu->setPressed(false);
    pressLock = isMouseHovered(this);
}
