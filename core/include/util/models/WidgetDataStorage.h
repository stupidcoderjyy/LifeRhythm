//
// Created by stupid_coder_jyy on 2024/3/10.
//

#ifndef LIFERHYTHM_WIDGETDATASTORAGE_H
#define LIFERHYTHM_WIDGETDATASTORAGE_H

#include "WidgetData.h"
#include "Identifier.h"
#include <QMap>

class WidgetDataStorage {
private:
    static WidgetDataStorage* instance;
    QMap<Identifier, WidgetData*> models;
public:
    static WidgetDataStorage* get();
    static void add(const Identifier& loc, WidgetData* model);
    static WidgetData* get(const Identifier& loc) noexcept;
protected:
    WidgetDataStorage();
};


#endif //LIFERHYTHM_WIDGETDATASTORAGE_H
