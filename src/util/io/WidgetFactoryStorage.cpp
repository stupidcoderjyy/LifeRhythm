//
// Created by stupid_coder_jyy on 2024/1/14.
//
#include "WidgetFactoryStorage.h"
#include "NBT.h"
#include <QDebug>
#include "CompileError.h"


WidgetFactoryStorage WidgetFactoryStorage::instance{};

WidgetFactoryStorage::WidgetFactoryStorage():
        ResourceManager({"factories", ".snbt"}, {"*.snbt"}){
}

WidgetFactory *WidgetFactoryStorage::load(const Identifier &loc) {
    auto* factory = new WidgetFactory(loc.getPath());
    try {
        auto* nbt = NBT::fromStringNbt(type.buildFilePath(loc));
        factory->setSource(nbt);
    } catch (CompileError& e) {
        delete factory;
        throw CompileError(e);
    }
    return factory;
}

void WidgetFactoryStorage::loadFailure(std::exception &e) noexcept {
    static_cast<CompileError&>(e).printErr();
}

WidgetFactoryStorage *WidgetFactoryStorage::getInstance() {
    return &instance;
}
