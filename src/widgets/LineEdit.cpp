//
// Created by stupid_coder_jyy on 2024/2/28.
//

#include "LineEdit.h"
#include "Styles.h"
#include "WidgetFactory.h"
#include "FocusManager.h"
#include <QApplication>
#include <QMouseEvent>

LineEdit::LineEdit(QWidget *parent): QLineEdit(parent) {
}

void LineEdit::onPreParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<LineEdit*>(target)->init();
    };
}

void LineEdit::onPostParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    WidgetFactory::parseTextWidget<LineEdit>(handlers, widgetTag);
}

void LineEdit::onStateRespondersParsing(StandardWidget::Handlers &responders, NBT *stateTag) {
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

void LineEdit::init() {
    setFrame(false);
    setFocusPolicy(Qt::ClickFocus);
    setContextMenuPolicy(Qt::NoContextMenu);
    setFont(Styles::FONT_MAIN);
    QPalette p = palette();
    p.setColor(QPalette::Text, QColor(Styles::GRAY_TEXT_0->rgbHex));
    setPalette(p);
    setObjectName("le");
    setFixedHeight(34);
    registerResponder(0, [this](QWidget* t){
        setTextMargins(2,2,2,0);
        setStyleSheet(qss_this(
                bg(Styles::CLEAR->rgbHex) + bd("1px", "solid", Styles::GRAY_2->rgbHex) + brad("3px")));
    });
    registerResponder(1, [this](QWidget* t){
        setTextMargins(1,1,1,0);
        setStyleSheet(qss_this(
                bg(Styles::CLEAR->rgbHex) + bd("2px", "solid", Styles::BLUE_1->rgbHex) + brad("3px")));
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
