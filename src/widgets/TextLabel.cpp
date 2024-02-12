//
// Created by stupid_coder_jyy on 2024/2/9.
//

#include "widgets/TextLabel.h"
#include "WidgetFactoryParsers.h"

TextLabel::TextLabel(QWidget *parent):QLabel(parent), StandardWidget() {
}

void TextLabel::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    WidgetFactoryParsers::parseTextWidget<QLabel>(handlers, widgetTag);
}

void TextLabel::onStateRespondersParsing(StandardWidget::Handlers &responders, NBT *stateTag) {
    onPostParsing(responders, stateTag);
}
