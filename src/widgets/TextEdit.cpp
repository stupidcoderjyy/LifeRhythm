//
// Created by stupid_coder_jyy on 2024/2/14.
//

#include "TextEdit.h"
#include "Styles.h"
#include "QApplication"
#include "WidgetFactory.h"
#include "QDebug"

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent) {
    setFrameStyle(NoFrame);
    setContextMenuPolicy(Qt::NoContextMenu);
    setFont(Styles::FONT_MAIN);
    setObjectName("te");
    setStyleSheet(qss_object("te", bg(Styles::CLEAR->rgbHex)));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vBar = new ScrollBar(this, Qt::Vertical);
    hBar = new ScrollBar(this, Qt::Horizontal);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, vBar, &ScrollBar::onValueSet);
    connect(vBar, &QScrollBar::valueChanged, [this](int v) { verticalScrollBar()->setValue(v); });
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, vBar, &ScrollBar::onRangeChanged);
    connect(horizontalScrollBar(), &QScrollBar::valueChanged, hBar, &ScrollBar::onValueSet);
    connect(hBar, &QScrollBar::valueChanged, [this](int v) { horizontalScrollBar()->setValue(v); });
    connect(horizontalScrollBar(), &QScrollBar::rangeChanged, hBar, &ScrollBar::onRangeChanged);
}

void TextEdit::onPostParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    WidgetFactory::parseTextWidget<TextEdit>(handlers, widgetTag);
}

void TextEdit::onStateRespondersParsing(StandardWidget::Handlers &responders, NBT *stateTag) {
    onPostParsing(responders, stateTag);
}

void TextEdit::resizeEvent(QResizeEvent *event) {
    hBar->setGeometry(0, height() - 8, width(), 7);
    vBar->setGeometry(width() - 8, 0, 7, height());
    QTextEdit::resizeEvent(event);
}

void TextEdit::mousePressEvent(QMouseEvent *e) {
    QTextEdit::mousePressEvent(e);
    QApplication::setCursorFlashTime(0);
}

void TextEdit::mouseReleaseEvent(QMouseEvent *e) {
    QTextEdit::mouseReleaseEvent(e);
    QApplication::setCursorFlashTime(1060);
}

