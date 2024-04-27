//
// Created by stupid_coder_jyy on 2024/4/27.
//

#include "DataA1.h"

USING_NAMESPACE(lr::log)

DataA1::DataA1(Usage *usage, bool active): usage(usage), active(active) {
}

DataA1::DataA1(): usage(), active() {
}

void DataA1::setUsage(Usage *u) {
    if (usage != u) {
        usage = u;
        emit sigDataChanged();
    }
}

void DataA1::setActive(bool a) {
    if (active != a) {
        active = a;
        emit sigDataChanged();
    }
}
