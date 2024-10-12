//
// Created by stupid_coder_jyy on 2024/3/4.
//

#include "WidgetStartPeriodMenu.h"
#include "WidgetUtil.h"
#include "RcManagers.h"

WidgetStartPeriodMenu::WidgetStartPeriodMenu(QWidget* parent): Widget(parent), configPages(),
        layoutPage(),recordTypeListWidget(), modelRecordTypes() {
}

void WidgetStartPeriodMenu::setRecordTypeData(ListData *m, QVector<QWidget*> *l) {
    modelRecordTypes = m;
    configPages = l;
    recordTypeListWidget = getPointer<RecordTypeListWidget>("recordTypeList");
    recordTypeListWidget->setData(modelRecordTypes);
    auto* container = getPointer<Widget>("pageContainer");
    connect(modelRecordTypes, &ListData::sigDataSelected, this, [this, container](int pre, int cur) {
        switchSingleLayoutWidget(container->layout(),
                                 pre < 0 ? nullptr : configPages->at(pre),
                                 configPages->at(cur));
    });
}

