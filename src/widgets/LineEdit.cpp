//
// Created by stupid_coder_jyy on 2024/2/28.
//

#include "LineEdit.h"
#include "Styles.h"
#include "WidgetFactory.h"
#include <QApplication>
#include <QMouseEvent>

LineEdit::LineEdit(QWidget *parent): QLineEdit(parent) {
    setFrame(false);
    setFocusPolicy(Qt::ClickFocus);
    setContextMenuPolicy(Qt::NoContextMenu);
    setFont(Styles::FONT_MAIN);
    setObjectName("le");
    setStyleSheet(qss_t("le", bg(Styles::CLEAR) + qss("color", Styles::GRAY_TEXT_0)));
}

void LineEdit::onPostParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    WidgetFactory::parseTextWidget<LineEdit>(handlers, widgetTag);
}

void LineEdit::onStateRespondersParsing(StandardWidget::Handlers &responders, NBT *stateTag) {
    onPostParsing(responders, stateTag);
}

void LineEdit::mousePressEvent(QMouseEvent *e) {
    QLineEdit::mousePressEvent(e);
    e->ignore();
    QApplication::setCursorFlashTime(0);
}

void LineEdit::mouseReleaseEvent(QMouseEvent *e) {
    QLineEdit::mouseReleaseEvent(e);
    QApplication::setCursorFlashTime(1060);
}

