//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "UsageType.h"
#include <utility>

USING_NAMESPACE(lr::log)

UsageType::UsageType(QColor color, QString name): color(std::move(color)), name(std::move(name)) {
}

UsageType::UsageType(const UsageType &o): color(o.color), name(o.name) {
}