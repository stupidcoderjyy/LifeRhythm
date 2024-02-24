//
// Created by stupid_coder_jyy on 2024/2/24.
//

#include "TimeBar.h"
#include "NBT.h"
#include <QRandomGenerator>
#include <QStringBuilder>

WidgetFactory* TimeBar::factory{};

TimeBar::TimeBar(QWidget *parent):RangeBar(true, parent){
    setBarRange(0, 1440);
    setVpp(3);
    setZoomRange(1, 5);
    setZoomStep(0.5);
}

RangeWidget *TimeBar::createRangeWidget() {
    return static_cast<RangeWidget*>(factory->apply());
}

void TimeBar::init() {
    Styles::initStyles();
    WidgetFactory::init();
    factory = WidgetFactory::fromNbt("rw", NBT::fromStringNbt(":/test/rw.snbt"));
    regClazz(factory, TimeBarRangeWidget);
    factory->parse();
}

TimeBarRangeWidget::TimeBarRangeWidget(QWidget *parent):RangeWidget(parent) {
}

void TimeBarRangeWidget::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        auto* rw = static_cast<TimeBarRangeWidget*>(target);
        rw->labelInfo = rw->getPointer<TextLabel>("info");
        rw->labelRange = rw->getPointer<TextLabel>("range");
    };
}

void TimeBarRangeWidget::syncDataToWidget() {
    RangeWidget::syncDataToWidget();
    auto* p = castedData<Period>();
    labelInfo->setText(p->getInfo());
    labelRange->setText(QString::asprintf("%02d:%02d-%02d:%02d",
            p->getBegin() / 60, p->getBegin() % 60,p->getEnd() / 60, p->getEnd() % 60));
}

void TimeBarRangeWidget::syncWidgetToData() {
    RangeWidget::syncWidgetToData();
    auto* p = castedData<Period>();
    p->setInfo(labelInfo->text());
}

void TimeBarRangeWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QString res;
    for (int i = 0 ; i < 10 ; i ++) {
        res = res % (char)QRandomGenerator::global()->bounded('a', 'z');
    }
    labelInfo->setText(res);
    syncWidgetToData();
    event->accept();
}
