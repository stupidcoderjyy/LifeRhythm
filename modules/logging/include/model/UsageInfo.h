//
// Created by stupid_coder_jyy on 2024/4/19.
//

#ifndef LIFERHYTHM_USAGEINFO_H
#define LIFERHYTHM_USAGEINFO_H

#include "Namespaces.h"
#include "WidgetData.h"

BEGIN_NAMESPACE(lr::log)

class UsageInfo : public WidgetData {
    friend class TimeUsageItem;
private:
    int usagePerc;
    QString usageName;
    QString usage;
public:
    UsageInfo();
    UsageInfo(int usagePerc, QString usageName, QString usage);
};

END_NAMESPACE

#endif //LIFERHYTHM_USAGEINFO_H
