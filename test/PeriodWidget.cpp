//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "PeriodWidget.h"
#include "WidgetFactory.h"
#include "RcManagers.h"
#include "Period.h"

void PeriodWidget::mainInit() {
    auto* p = WidgetFactoryStorage::get("log:timebar/period");
    regClazz(p, PeriodWidget);
}

PeriodWidget::PeriodWidget(QWidget *parent): RangeWidget(parent), labelInfo(), labelRange() {
}

void PeriodWidget::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<PeriodWidget*>(target)->init();
    };
}

void PeriodWidget::init() {
    labelInfo = getPointer<TextLabel>("labelInfo");
    labelRange = getPointer<TextLabel>("labelRange");
}

void PeriodWidget::syncDataToWidget() {
    RangeWidget::syncDataToWidget();
    if (wData) {
        show();
        auto* d = wData->cast<Period>();
        labelInfo->setText(d->info);
        labelRange->setText(QString::asprintf("%02d:%02d-%02d:%02d", d->begin / 60, d->begin % 60, d->end / 60, d->end % 60));
    } else {
        close();
    }
}
