//
// Created by JYY on 24-12-24.
//

#include "ImageRcLoader.h"
#include <QPixmap>

USING_LR

QPixmap *ImageRcLoader::load(const Identifier &loc, const QString &absolutePath) {
    return new QPixmap(absolutePath);
}
