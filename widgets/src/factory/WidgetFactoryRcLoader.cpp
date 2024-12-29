//
// Created by JYY on 24-12-24.
//

#include "WidgetFactoryRcLoader.h"
#include "NBT.h"
#include "CompileError.h"

USING_LR

WidgetFactory *WidgetFactoryRcLoader::load(const Identifier &loc, const QString &absolutePath) {
    auto* nbt = NBT::fromStringNbt(absolutePath);
    auto name = loc.getPath();
    if (const int i = name.lastIndexOf('/'); i > 0) {
        name = name.mid(i + 1, name.length() - i - 1);
    }
    return WidgetFactory::fromNbt(name, nbt);
}

void WidgetFactoryRcLoader::onLoadFailed(std::exception &e) noexcept {
    static_cast<CompileError&>(e).printErr();
}