//
// Created by stupid_coder_jyy on 2024/1/16.
//

#include "ResourceManager.h"

ResourceType::ResourceType(QString pathPrefix, QString fileSuffix) :
        pathPrefix(std::move(pathPrefix)),
        pathSuffix(std::move(fileSuffix)) {
}

Identifier ResourceType::buildFilePath(const Identifier &loc) const {
    return loc.withPrefix(pathPrefix + "/").withSuffix(pathSuffix);
}

Identifier ResourceType::buildDirPath(const Identifier &loc) const {
    if (loc.getPath().isEmpty()) {
        return {loc.getNamespace(), pathPrefix};
    }
    return loc.withPrefix(pathPrefix + "/");
}

const QString &ResourceType::getPathPrefix() const {
    return pathPrefix;
}

const QString &ResourceType::getPathSuffix() const {
    return pathSuffix;
}

ResourceType::ResourceType(ResourceType &&o) noexcept {
    pathPrefix = std::move(o.pathPrefix);
    pathSuffix = std::move(o.pathSuffix);
}