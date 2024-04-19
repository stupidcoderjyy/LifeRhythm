//
// Created by stupid_coder_jyy on 2024/4/19.
//

#ifndef LIFERHYTHM_TIMEUSAGE_H
#define LIFERHYTHM_TIMEUSAGE_H

#include "Namespaces.h"
#include "WidgetData.h"

BEGIN_NAMESPACE(lr::log)

class TimeUsage : public WidgetData {
    friend class TimeUsageItem;
private:
    int usagePerc;
    QString usageName;
    QString usage;
public:
    TimeUsage();
    TimeUsage(int usagePerc, QString usageName, QString usage);
};

END_NAMESPACE

#endif //LIFERHYTHM_TIMEUSAGE_H
