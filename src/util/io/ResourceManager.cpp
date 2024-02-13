//
// Created by stupid_coder_jyy on 2024/1/16.
//

#include "ResourceManager.h"

ResourceType::ResourceType(QString typeDir, QString fileType) :
        typeDir(std::move(typeDir)),
        fileType(std::move(fileType)) {
}

QString ResourceType::buildPath(const Identifier &loc) const {
    if (typeDir.isEmpty()) {
        return loc.getNamespace() + "/" + loc.getPath() + fileType;
    }
    return loc.getNamespace() + "/" + typeDir + "/" + loc.getPath() + fileType;
}

const QString &ResourceType::getTypeDir() const {
    return typeDir;
}

const QString &ResourceType::getFileType() const {
    return fileType;
}

ResourceType::ResourceType(ResourceType &&o) noexcept {
    typeDir = std::move(o.typeDir);
    fileType = std::move(o.fileType);
}

ResourceType::ResourceType(const ResourceType &o) {
    typeDir = o.typeDir;
    fileType = o.fileType;
}
