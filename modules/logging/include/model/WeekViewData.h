//
// Created by stupid_coder_jyy on 2024/4/15.
//

#ifndef LIFERHYTHM_WEEKVIEWDATA_H
#define LIFERHYTHM_WEEKVIEWDATA_H

#include "Namespaces.h"
#include "NestedListData.h"
#include <QDate>

BEGIN_NAMESPACE(lr::log)

class WeekViewData : public WidgetData {
public: //TODO visible for testing
    QDate weekBegin;
    NestedListData periods;
public:
    WeekViewData();
};

END_NAMESPACE

#endif //LIFERHYTHM_WEEKVIEWDATA_H
