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
#include "PeriodTypeIcon.h"
#include "PeriodTypeColorBox.h"
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
    auto* f = WidgetFactoryStorage::get("log:widget_maintab");
    regClazz(f, MainTab);
    regClazz(f, TimeBar);
    f = WidgetFactoryStorage::get("log:widget_mainpage");
    regClazz(f, PeriodTypeTreeWidget);
    regClazz(f, PeriodTypeIcon);
    f = WidgetFactoryStorage::get("log:item_period_type");
    regClazz(f, PeriodTypeIcon);
    regClazz(f, PeriodTypeTreeItem);
    f = WidgetFactoryStorage::get("log:widget_content");
    regClazz(f, WidgetStartPeriodMenu);
    regClazz(f, RecordTypeListWidget);
    f = WidgetFactoryStorage::get("log:item_timebar");
    regClazz(f, TimeBarItem);
    f = WidgetFactoryStorage::get("log:item_record_type");
    regClazz(f, RecordTypeItem);
    f = WidgetFactoryStorage::get("log:widget_create_type");
    regClazz(f, PeriodTypeColorBox);
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
