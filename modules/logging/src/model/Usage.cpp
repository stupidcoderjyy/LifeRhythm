//
// Created by stupid_coder_jyy on 2024/4/24.
//

#include "Usage.h"
#include <utility>

USING_NAMESPACE(lr::log)

Usage::Usage(UsageType *type, QString name, int duration):
        TreeNode(), type(type), name(std::move(name)), duration(duration) {
}

Usage::Usage(): TreeNode(), type(), name(), duration() {
}