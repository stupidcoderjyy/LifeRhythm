//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "WidgetA.h"
#include "RcManagers.h"
#include "Task.h"
#include "WidgetStartPeriodMenu.h"
#include "LifeRhythm.h"
#include "Period.h"
#include "RecordType.h"
#include "PeriodTypeIcon.h"
#include "PeriodTypeColorBox.h"
#include <QThread>

USING_NAMESPACE(lr::log)

WidgetA::WidgetA(QWidget *parent): TabWidget(parent), loader() {
}

void WidgetA::onTabCreated() {
    bar = getPointer<TimeBar>("timeBar");
    loader = new PeriodDataLoader("test.dat");
    auto* loadTask = new Task([this](auto fi){
        loader->load();
    });
    loadTask->onFinished(this, [this](){
        auto *d = loader->getData()->cast<PeriodList>();
        bar->setData(d);
        if (d->getData().isEmpty()) {
            d->beginEdit();
            d->append(new Period(4 * 60, 5 * 60, "aaaaa"));
            d->append(new Period(6 * 60 + 30, 8 * 60, "bbbbbb"));
            d->endEdit();
        }
        bar->syncDataToWidget();
    });
    loadTask->start();
}
