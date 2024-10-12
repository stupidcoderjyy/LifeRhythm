//
// Created by stupid_coder_jyy on 2024/2/14.
//

#include "RcManagers.h"
#include "RcLoaders.h"
#include "RcTypes.h"

STATIC_INSTANCE(ImageStorage)
STATIC_INSTANCE(WidgetFactoryStorage)
STATIC_INSTANCE(StyleGroupStorage)

ImageStorage::ImageStorage() {
    _addManager(new BuiltInResourceManager(RcTypes::IMG, ImageRcLoader::get()));
}

WidgetFactoryStorage::WidgetFactoryStorage() {
    _addManager(new BuiltInResourceManager(RcTypes::FACTORY, WidgetFactoryRcLoader::get()));
}

void WidgetFactoryStorage::parseAll() {
    for (ResourceManager<WidgetFactory>* m : instance.managers) {
        for (WidgetFactory* f : m->getData()) {
            f->parse();
        }
    }
}

StyleGroupStorage::StyleGroupStorage() {
    _addManager(new BuiltInResourceManager(RcTypes::STYLE_GROUP, StyleGroupRcLoader::get()));
}
