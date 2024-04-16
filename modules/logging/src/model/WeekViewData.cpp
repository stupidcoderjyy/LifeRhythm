//
// Created by stupid_coder_jyy on 2024/4/15.
//

#include "WeekViewData.h"

USING_NAMESPACE(lr::log)

WeekViewData::WeekViewData(): WidgetData(), weekBegin(), periods() {
    periods.makeRow(7);
}