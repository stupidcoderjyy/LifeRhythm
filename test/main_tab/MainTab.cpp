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
    auto* mt = WidgetFactoryStorage::get("log:widget_maintab");
    regClazz(mt, MainTab);
    regClazz(mt, TimeBar);
    TimeBarItem::mainInit();
    WidgetStartPeriodMenu::mainInit();
    auto* mp = WidgetFactoryStorage::get("log:dialog_start_record/widget_mainpage");
    regClazz(mp, PeriodTypeTreeWidget);
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
    auto* content = WidgetFactoryStorage::get("log:dialog_start_record/widget_content")
            ->applyAndCast<WidgetStartPeriodMenu>();
    content->setRecordTypeData(modelRecordTypeList, &startRecordMenuPages);

    auto* pageMain = WidgetFactoryStorage::get("log:dialog_start_record/widget_mainpage")->apply();
    addStartPeriodType("lr:dialog_close", "A", pageMain);

    lr::LifeRhythm::generateTitledDialog("开始记录", content);
}
