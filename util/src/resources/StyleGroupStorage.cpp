//
// Created by JYY on 24-12-24.
//

#include "StyleGroupStorage.h"
#include "RcTypes.h"
#include "StyleGroupRcLoader.h"

USING_LR

STATIC_INSTANCE(StyleGroupStorage)

StyleGroupStorage::StyleGroupStorage() {
    _addManager(new BuiltInResourceManager(RcTypes::STYLE_GROUP, StyleGroupRcLoader::get()));
}
