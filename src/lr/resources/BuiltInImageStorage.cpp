//
// Created by stupid_coder_jyy on 2024/2/7.
//

#include "BuiltInImageStorage.h"

BuiltInImageStorage BuiltInImageStorage::instance{};

BuiltInImageStorage *BuiltInImageStorage::getInstance() {
    return &instance;
}

BuiltInImageStorage::BuiltInImageStorage():
        BuiltInResourceManager({"images", ".png"}) {
}

QPixmap *BuiltInImageStorage::load(const Identifier &loc, const QString& absolutePath) {
    return new QPixmap(absolutePath);
}
