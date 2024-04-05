//
// Created by stupid_coder_jyy on 2024/3/30.
//

#include "ModuleLog.h"
#include "RcManagers.h"
#include "WidgetA.h"
#include "TimeBar.h"
#include "PeriodTypeIcon.h"
#include "PeriodTypeTreeWidget.h"
#include "WidgetStartPeriodMenu.h"
#include "PeriodTypeColorBox.h"
#include "LifeRhythm.h"

USING_NAMESPACE(lr::log)

ModuleLog::ModuleLog(): Module("log"), tabMain() {
}

void ModuleLog::mainInit() {
    auto* f = WidgetFactoryStorage::get("log:wg_a");
    regClazz(f, WidgetA);
    regClazz(f, TimeBar);
    f = WidgetFactoryStorage::get("log:widget_mainpage");
    regClazz(f, PeriodTypeTreeWidget);
    regClazz(f, PeriodTypeIcon);
    f = WidgetFactoryStorage::get("log:item_periodtype");
    regClazz(f, PeriodTypeIcon);
    regClazz(f, PeriodTypeTreeItem);
    f = WidgetFactoryStorage::get("log:widget_content");
    regClazz(f, WidgetStartPeriodMenu);
    regClazz(f, RecordTypeListWidget);
    f = WidgetFactoryStorage::get("log:item_timebar");
    regClazz(f, TimeBarItem);
    f = WidgetFactoryStorage::get("log:item_recordtype");
    regClazz(f, RecordTypeItem);
    f = WidgetFactoryStorage::get("log:widget_newtype");
    regClazz(f, PeriodTypeColorBox);
}

void ModuleLog::postInit() {
    tabMain = WidgetFactoryStorage::get("log:wg_a")->applyAndCast<WidgetA>();
    LifeRhythm::insertTab("测试", tabMain);
}