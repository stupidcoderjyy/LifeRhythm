//
// Created by stupid_coder_jyy on 2024/4/15.
//

#include "TimeBarItem.h"
#include "Period.h"
#include <QPainterPath>

USING_NAMESPACE(lr::log)

TimeBarItem::TimeBarItem(QWidget *parent): BarItem(parent), labelInfo(), labelRange() {
    colorBg = Styles::GRAY_1->color;
}

void TimeBarItem::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<TimeBarItem*>(target)->init();
    };
}

void TimeBarItem::init() {
    labelInfo = getPointer<TextLabel>("labelInfo");
    labelRange = getPointer<TextLabel>("labelRange");
}

void TimeBarItem::syncDataToWidget() {
    BarItem::syncDataToWidget();
    if (wData) {
        show();
        auto* d = wData->cast<Period>();
        labelInfo->setText(d->info);
        labelRange->setText(QString::asprintf(
                "%02d:%02d-%02d:%02d",
                d->getBegin() / 60,
                d->getBegin() % 60,
                d->getEnd() / 60,
                d->getEnd() % 60));
    } else {
        close();
    }
}

void TimeBarItem::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(QRect(4, 2, width() - 8, height() - 4), 3, 3);
    p.fillPath(path, colorBg);
}