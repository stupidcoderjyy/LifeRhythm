//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "MainTab.h"
#include "WidgetFactory.h"
#include "RcManagers.h"
#include "PeriodWidget.h"
#include "Period.h"
#include "Task.h"

MainTab::MainTab(QWidget *parent): lr::TabWidget(parent), loader(){
}

void MainTab::mainInit() {
    auto* mt = WidgetFactoryStorage::get("log:maintab");
    regClazz(mt, MainTab);
    regClazz(mt, TimeBar);
    PeriodWidget::mainInit();
}

void MainTab::onTabCreated() {
    bar = getPointer<TimeBar>("timeBar");
    loader = new PeriodDataLoader("test.dat");
    auto* loadTask = new Task([this](auto fi){
        loader->load();
    });
    loadTask->onFinished(this, [this]() {
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
