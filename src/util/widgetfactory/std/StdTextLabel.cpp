//
// Created by stupid_coder_jyy on 2024/2/9.
//

#include "StdTextLabel.h"
#include "WidgetFactoryParsers.h"

StdTextLabel::StdTextLabel(QWidget *parent):QLabel(parent), StandardWidget() {
}

void StdTextLabel::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    WidgetFactoryParsers::parseTextWidget<QLabel>(handlers, widgetTag);
}

void StdTextLabel::onStateRespondersParsing(StandardWidget::Handlers &responders, NBT *stateTag) {
    onPostParsing(responders, stateTag);
}
