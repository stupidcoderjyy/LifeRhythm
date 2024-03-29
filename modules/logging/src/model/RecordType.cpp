//
// Created by stupid_coder_jyy on 2024/3/5.
//

#include "RecordType.h"

RecordType::RecordType(QString iconPath, QString name): WidgetData(),
        iconPath(std::move(iconPath)), name(std::move(name)) {
}