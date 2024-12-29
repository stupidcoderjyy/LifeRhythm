//
// Created by JYY on 24-12-25.
//

#include "WidgetFactoryStorage.h"
#include "RcTypes.h"
#include "WidgetFactoryRcLoader.h"

USING_LR

STATIC_INSTANCE(WidgetFactoryStorage)

WidgetFactoryStorage::WidgetFactoryStorage() {
    _addManager(new BuiltInResourceManager(RcTypes::FACTORY, WidgetFactoryRcLoader::get()));
}

void WidgetFactoryStorage::parseAll() {
    for (ResourceManager<WidgetFactory> *m: instance.managers) {
        for (WidgetFactory *f: m->getData()) {
            f->parse();
        }
    }
}