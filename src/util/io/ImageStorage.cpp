//
// Created by stupid_coder_jyy on 2024/2/7.
//

#include "ImageStorage.h"
#include <QDebug>

ImageStorage ImageStorage::instance{};

ImageStorage *ImageStorage::getInstance() {
    return &instance;
}

ImageStorage::ImageStorage():
        ResourceManager({"images", ".png"}, {"*.png"}) {
}

QPixmap *ImageStorage::load(const Identifier &loc) {
    auto* img = new QPixmap(loc.toFullPath());
    qDebug() << loc.toString() << img;
    return img;
}
