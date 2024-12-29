//
// Created by stupid_coder_jyy on 2024/3/10.
//

#include "WidgetDataStorage.h"

USING_LR

WidgetDataStorage* WidgetDataStorage::instance = new WidgetDataStorage();

WidgetDataStorage *WidgetDataStorage::get() {
    return instance;
}

void WidgetDataStorage::add(const Identifier &loc, WidgetData *model) {
    instance->models.insert(loc, model);
}

WidgetData *WidgetDataStorage::get(const Identifier &loc) noexcept {
    return instance->models.value(loc);
}

WidgetDataStorage::WidgetDataStorage() = default;
