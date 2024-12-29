//
// Created by JYY on 24-12-24.
//

#include "ImageStorage.h"
#include "ImageRcLoader.h"
#include "RcTypes.h"

USING_LR

STATIC_INSTANCE(ImageStorage)

ImageStorage::ImageStorage() {
    _addManager(new BuiltInResourceManager<QPixmap>(RcTypes::IMG, ImageRcLoader::get()));
}
