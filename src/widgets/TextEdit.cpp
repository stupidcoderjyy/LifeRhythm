//
// Created by stupid_coder_jyy on 2024/2/14.
//

#include "TextEdit.h"
#include "Styles.h"
#include "QApplication"

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent) {
    setFrameStyle(NoFrame);
    setContextMenuPolicy(Qt::NoContextMenu);
    setFont(Styles::FONT_MAIN);
    QPalette p = palette();
    p.setBrush(QPalette::Base, QColor(Styles::BLACK));
    p.setBrush(QPalette::Text, QColor(Styles::GRAY_TEXT_0));
    p.setBrush(QPalette::Highlight, QColor(Styles::CYAN_DARK));
    p.setBrush(QPalette::HighlightedText, Qt::NoBrush);
    setPalette(p);
    setAutoFillBackground(true);
    ANIMATED_SCROLL_CLAZZ_CONSTRUCTOR
}

ANIMATED_SCROLL_CLAZZ_OVERRIDES(TextEdit, QTextEdit)

void TextEdit::mousePressEvent(QMouseEvent *e) {
    QTextEdit::mousePressEvent(e);
    QApplication::setCursorFlashTime(0);
}

void TextEdit::mouseReleaseEvent(QMouseEvent *e) {
    QTextEdit::mouseReleaseEvent(e);
    QApplication::setCursorFlashTime(1060);
}
