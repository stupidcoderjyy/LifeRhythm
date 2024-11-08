//
// Created by stupid_coder_jyy on 2024/2/27.
//

#include "LineEditOptionsBox.h"
#include "QHBoxLayout"
#include "RcManagers.h"
#include "WidgetUtil.h"
#include "FocusManager.h"
#include <QApplication>
#include <QMouseEvent>

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
    setStyleSheet(qss_this(bg(Styles::CLEAR->rgbHex) + qss("color", Styles::GRAY_TEXT_0->rgbHex)));
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

LineEditOptionsBox::LineEditOptionsBox(QWidget *parent): OptionsBox(parent) {
    auto* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(5, 0, 8, 0);
    setLayout(layout);
    optionEditor = new OptionsBoxLineEdit(this);
    optionEditor->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    buttonOpenMenu = new DisplayOptionsButton(this);
    layout->addWidget(optionEditor);
    layout->addWidget(buttonOpenMenu);
    setMinimumWidth(200);
    setFixedHeight(40);
    setContentsMargins(0, 0, 0, 0);
    connect(buttonOpenMenu, &ArrowButton::sigActivated, this, &LineEditOptionsBox::clickBox);
}

void LineEditOptionsBox::onMenuClose() {
    OptionsBox::onMenuClose();
    buttonOpenMenu->setPressed(false);
}

void LineEditOptionsBox::clickBox(bool pressed) {
    if (pressed && pressLock) {
        buttonOpenMenu->setPressed(false);
    }
    OptionsBox::clickBox(pressed);
}