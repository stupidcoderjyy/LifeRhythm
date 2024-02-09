//
// Created by stupid_coder_jyy on 2024/2/9.
//

#include "StdTextLabel.h"
#include "WidgetFactoryParsers.h"

StdTextLabel::StdTextLabel(QWidget *parent):QLabel(parent), StandardWidget() {
}

void StdTextLabel::postParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    WidgetFactoryParsers::parseTextWidget<QLabel>(handlers, widgetTag);
}
