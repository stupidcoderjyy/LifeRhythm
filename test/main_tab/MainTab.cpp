//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "MainTab.h"
#include "WidgetFactory.h"
#include "RcManagers.h"
#include "Task.h"
#include "SelectableListData.h"
#include "WidgetStartPeriodMenu.h"
#include "LifeRhythm.h"
#include "Period.h"
#include "PeriodTypeTreeWidget.h"
#include "RecordType.h"
#include <QThread>
#include <utility>

MainTab::MainTab(QWidget *parent):
        lr::TabWidget(parent), loader(), modelRecordTypeList(), startRecordMenuPages(){
}

void MainTab::addStartPeriodType(QString icon, QString typeName, QWidget *menuPage) {
    modelRecordTypeList->append(new RecordType(std::move(icon), std::move(typeName)));
    startRecordMenuPages.append(menuPage);
}

void MainTab::mainInit() {
    auto* f0 = WidgetFactoryStorage::get("log:widget_maintab");
    auto* f1 = WidgetFactoryStorage::get("log:widget_mainpage");
    auto* f2 = WidgetFactoryStorage::get("log:item_period_type");
    auto* f3 = WidgetFactoryStorage::get("log:widget_content");
    auto* f4 = WidgetFactoryStorage::get("log:item_timebar");
    auto* f5 = WidgetFactoryStorage::get("log:item_record_type");
    regClazz(f0, MainTab);
    regClazz(f0, TimeBar);
    regClazz(f1, PeriodTypeTreeWidget);
    regClazz(f2, PeriodTypeIcon);
    regClazz(f2, PeriodTypeTreeItem);
    regClazz(f3, WidgetStartPeriodMenu);
    regClazz(f3, RecordTypeListWidget);
    regClazz(f4, TimeBarItem);
    regClazz(f5, RecordTypeItem);
}

void MainTab::onTabCreated() {
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
    modelRecordTypeList = new SelectableListData();
    auto* content = WidgetFactoryStorage::get("log:widget_content")
            ->applyAndCast<WidgetStartPeriodMenu>();
    content->setRecordTypeData(modelRecordTypeList, &startRecordMenuPages);

    auto* pageMain = WidgetFactoryStorage::get("log:widget_mainpage")->apply();
    addStartPeriodType("lr:dialog_close", "A", pageMain);

    lr::LifeRhythm::generateTitledDialog("开始记录", content);
}
