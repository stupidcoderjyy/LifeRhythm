//
// Created by stupid_coder_jyy on 2024/3/4.
//

#include "WidgetStartPeriodMenu.h"
#include "WidgetUtil.h"
#include "RcManagers.h"

void WidgetStartPeriodMenu::mainInit() {
    regClazz(WidgetFactoryStorage::get("log:record_type_item"), RecordTypeItem);
    auto* f = WidgetFactoryStorage::get("log:dialog_content_start_record");
    regClazz(f, WidgetStartPeriodMenu);
    regClazz(f, RecordTypeListWidget);
}

WidgetStartPeriodMenu::WidgetStartPeriodMenu(QWidget* parent):
        Widget(parent), configPages(), recordTypeListWidget(), modelRecordTypes() {
}

void WidgetStartPeriodMenu::setRecordTypeData(SelectableListData *m, QVector<QWidget*> *l) {
    modelRecordTypes = m;
    configPages = l;
    recordTypeListWidget = getPointer<RecordTypeListWidget>("recordTypeList");
    recordTypeListWidget->setData(modelRecordTypes);
    auto* container = getPointer<Widget>("pageContainer");
    connect(modelRecordTypes, &SelectableListData::sigDataSelected, this, [this, container](int pre, int cur) {
        switchSingleLayoutWidget(container->layout(),
                                 pre < 0 ? nullptr : configPages->at(pre),
                                 configPages->at(cur));
    });
}

