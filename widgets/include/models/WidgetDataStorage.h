//
// Created by stupid_coder_jyy on 2024/3/10.
//

#ifndef WIDGETDATASTORAGE_H
#define WIDGETDATASTORAGE_H

#include "WidgetData.h"
#include "Identifier.h"
#include <QMap>

BEGIN_LR

class CORE_API WidgetDataStorage {
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

END_LR

#endif //WIDGETDATASTORAGE_H
