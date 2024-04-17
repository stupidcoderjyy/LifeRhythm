//
// Created by stupid_coder_jyy on 2024/3/30.
//

#include "ModuleLog.h"
#include "RcManagers.h"
#include "A.h"
#include "TimeBar.h"
#include "PeriodTypeIcon.h"
#include "PeriodTypeTreeWidget.h"
#include "WidgetStartPeriodMenu.h"
#include "PeriodTypeColorBox.h"
#include "TimeBarItem.h"
#include "LifeRhythm.h"
#include "B.h"
#include "B0.h"
#include "B1.h"
#include "DateRangeSelector.h"

USING_NAMESPACE(lr::log)

ModuleLog::ModuleLog(): Module("log"), tabMain(), tabProfiles() {
}

void ModuleLog::mainInit() {
    auto* f = WidgetFactoryStorage::get("log:wg_a");
    regClazz(f, A);
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
    f = WidgetFactoryStorage::get("log:wg_b");
    regClazz(f, B);
    regClazz(f, B0);
    regClazz(f, B1);
    regClazz(f, B4);
    regClazz(f, B3);
    regClazz(f, DateRangeSelector);
}

void ModuleLog::postInit() {
    tabMain = WidgetFactoryStorage::get("log:wg_a")->applyAndCast<A>();
    LifeRhythm::insertTab("测试", tabMain);
    tabProfiles = WidgetFactoryStorage::get("log:wg_b")->applyAndCast<B>();
    LifeRhythm::insertTab("档案", tabProfiles);
}
