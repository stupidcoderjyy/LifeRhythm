//
// Created by stupid_coder_jyy on 2024/2/28.
//

#include "LineEdit.h"
#include "Styles.h"
#include "WidgetFactory.h"
#include "FocusManager.h"
#include <QApplication>
#include <QMouseEvent>

LineEdit::LineEdit(QWidget *parent): QLineEdit(parent), ready() {
    QPalette p = palette();
    p.setColor(QPalette::Highlight, Styles::BLUE_1->color);
    p.setColor(QPalette::Text, Styles::GRAY_TEXT_0->color);
    setPalette(p);
}

void LineEdit::onPreParsing(Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<LineEdit*>(target)->init();
    };
}

void LineEdit::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    WidgetFactory::parseTextWidget<LineEdit>(handlers, widgetTag);
}

void LineEdit::onStateRespondersParsing(Handlers &responders, NBT *stateTag) {
    onPostParsing(responders, stateTag);
}

void LineEdit::mousePressEvent(QMouseEvent *e) {
    QLineEdit::mousePressEvent(e);
    QApplication::setCursorFlashTime(0);
    FocusManager::mark(this);
}

void LineEdit::mouseReleaseEvent(QMouseEvent *e) {
    QLineEdit::mouseReleaseEvent(e);
    QApplication::setCursorFlashTime(1060);
}

void LineEdit::resizeEvent(QResizeEvent *event) {
    init();
}

void LineEdit::init() {
    if (ready) {
        return;
    }
    ready = true;
    setFrame(false);
    setFocusPolicy(Qt::ClickFocus);
    setContextMenuPolicy(Qt::NoContextMenu);
    setFont(Styles::FONT_MAIN);
    setObjectName("le");
    setFixedHeight(35);
    registerResponder(0, [this](auto){
        setStyleSheet(qss_this(
                bg(Styles::CLEAR->rgbHex) +
                bd("1px", "solid", Styles::GRAY_2->rgbHex) +
                brad("3px")));
    });
    registerResponder(1, [this](auto){
        setStyleSheet(qss_this(
                bg(Styles::CLEAR->rgbHex) +
                bd("2px", "solid", Styles::BLUE_1->rgbHex) +
                brad("3px")));
    });
    setState(0);
}

void LineEdit::focusInEvent(QFocusEvent *event) {
    setState(1);
    QLineEdit::focusInEvent(event);
}

void LineEdit::focusOutEvent(QFocusEvent *event) {
    setState(0);
    FocusManager::mark(nullptr);
    QLineEdit::focusOutEvent(event);
}
