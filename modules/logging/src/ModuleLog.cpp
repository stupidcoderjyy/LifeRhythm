//
// Created by stupid_coder_jyy on 2024/3/30.
//

#include "ModuleLog.h"
#include "RcManagers.h"
#include "A.h"
#include "A0.h"
#include "UsageTypeIcon.h"
#include "UsageTypeTreeWidget.h"
#include "WidgetStartPeriodMenu.h"
#include "PeriodTypeColorBox.h"
#include "TimeBarItem.h"
#include "LifeRhythm.h"
#include "B.h"
#include "B0.h"
#include "B1.h"
#include "DateRangeSelector.h"
#include "B2.h"
#include "B3.h"
#include "B4.h"
#include "UsageTypeLabel.h"
#include "A1.h"

USING_NAMESPACE(lr::log)

ModuleLog::ModuleLog(): Module("log"), tabMain(), tabProfiles() {
}

void ModuleLog::mainInit() {
    auto* f = WidgetFactoryStorage::get("log:wg_a");
    regClazz(f, A);
    regClazz(f, A0);
    regClazz(f, A1);
    f = WidgetFactoryStorage::get("log:widget_mainpage");
    regClazz(f, UsageTypeTreeWidget);
    regClazz(f, UsageTypeIcon);
    f = WidgetFactoryStorage::get("log:item_periodtype");
    regClazz(f, UsageTypeIcon);
    regClazz(f, UsageTypeTreeItem);
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
    regClazz(f, B2);
    regClazz(f, B4);
    regClazz(f, B3);
    regClazz(f, DateRangeSelector);
    f = WidgetFactoryStorage::get("log:item_a1");
    regClazz(f, UsageTypeLabel);
    regClazz(f, ItemA1);
}

void ModuleLog::postInit() {
    tabMain = WidgetFactoryStorage::get("log:wg_a")->applyAndCast<A>();
    LifeRhythm::insertTab("测试", tabMain);
    tabProfiles = WidgetFactoryStorage::get("log:wg_b")->applyAndCast<B>();
    LifeRhythm::insertTab("档案", tabProfiles);
}
