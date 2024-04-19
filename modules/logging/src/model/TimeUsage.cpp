//
// Created by stupid_coder_jyy on 2024/4/19.
//

#include "TimeUsage.h"
#include <utility>

USING_NAMESPACE(lr::log)

TimeUsage::TimeUsage(): WidgetData() {
}

TimeUsage::TimeUsage(int usagePerc, QString usageName, QString usage):
        WidgetData(), usagePerc(usagePerc), usageName(std::move(usageName)), usage(std::move(usage)){
}