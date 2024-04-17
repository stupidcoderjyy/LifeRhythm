//
// Created by stupid_coder_jyy on 2024/2/9.
//

#include "TextLabel.h"
#include "WidgetFactory.h"
#include <QMouseEvent>

TextLabel::TextLabel(QWidget *parent):QLabel(parent), StandardWidget() {
    setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setFont(Styles::FONT_MAIN);
    QPalette p = palette();
    p.setColor(QPalette::WindowText, QColor(Styles::GRAY_TEXT_0->rgbHex));
    p.setColor(QPalette::Text, QColor(Styles::GRAY_TEXT_0->rgbHex));
    setPalette(p);
}

void TextLabel::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    WidgetFactory::parseTextWidget<QLabel>(handlers, widgetTag);
}

void TextLabel::onStateRespondersParsing(StandardWidget::Handlers &responders, NBT *stateTag) {
    onPostParsing(responders, stateTag);
}

void TextLabel::mousePressEvent(QMouseEvent *ev) {
    ev->ignore();
}

void TextLabel::mouseReleaseEvent(QMouseEvent *ev) {
    ev->ignore();
}
