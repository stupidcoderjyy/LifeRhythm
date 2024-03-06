//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "MainTab.h"
#include "WidgetFactory.h"
#include "RcManagers.h"
#include "PeriodWidget.h"
#include "Period.h"
#include "Task.h"
#include "SelectableListData.h"
#include "RecordType.h"
#include "WidgetStartPeriodMenu.h"
#include "LifeRhythm.h"
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
    auto* mt = WidgetFactoryStorage::get("log:maintab");
    regClazz(mt, MainTab);
    regClazz(mt, TimeBar);
    PeriodWidget::mainInit();
    WidgetStartPeriodMenu::mainInit();
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
    auto* content = WidgetFactoryStorage::get("log:dialog_content_start_record")
            ->applyAndCast<WidgetStartPeriodMenu>();
    content->setRecordTypeData(modelRecordTypeList, &startRecordMenuPages);
    auto* l1 = new QLabel;
    auto* l2 = new QLabel;
    l1->setStyleSheet(bg(Styles::GRAY_2));
    l2->setStyleSheet(bg(Styles::BLUE_1));
    addStartPeriodType("lr:dialog_close", "A", l1);
    addStartPeriodType("lr:icon_30", "B", l2);

    lr::LifeRhythm::generateTitledDialog("开始记录", content);
}
