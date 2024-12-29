//
// Created by JYY on 24-12-24.
//

#ifndef IMAGESTORAGE_H
#define IMAGESTORAGE_H

#include "Identifier.h"
#include "RcManager.h"

BEGIN_LR

class CORE_API ImageStorage : public MultiSourceResourceManager<QPixmap> {
    STATIC_DEFINE(ImageStorage, QPixmap)
};

END_LR

#endif //IMAGESTORAGE_H
