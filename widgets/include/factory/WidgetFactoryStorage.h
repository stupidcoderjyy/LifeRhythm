//
// Created by JYY on 24-12-25.
//

#ifndef WIDGETFACTORYSTORAGE_H
#define WIDGETFACTORYSTORAGE_H

#include "RcManager.h"
#include "WidgetFactory.h"
#include "Plugin.h"

BEGIN_LR

class CORE_API WidgetFactoryStorage : public MultiSourceResourceManager<WidgetFactory>{
    STATIC_DEFINE(WidgetFactoryStorage, WidgetFactory)
public:
    static void parseAll();
};

END_LR

#endif //WIDGETFACTORYSTORAGE_H
