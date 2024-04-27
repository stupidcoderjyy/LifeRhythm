//
// Created by stupid_coder_jyy on 2024/4/19.
//

#include "UsageInfo.h"
#include <utility>

USING_NAMESPACE(lr::log)

UsageInfo::UsageInfo(): WidgetData() {
}

UsageInfo::UsageInfo(int usagePerc, QString usageName, QString usage):
        WidgetData(), usagePerc(usagePerc), usageName(std::move(usageName)), usage(std::move(usage)){
}