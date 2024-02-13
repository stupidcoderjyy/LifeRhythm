//
// Created by stupid_coder_jyy on 2024/1/14.
//
#include "lr/resources/BuiltInWidgetFactoryStorage.h"
#include "NBT.h"
#include <QDebug>
#include "CompileError.h"


BuiltInWidgetFactoryStorage BuiltInWidgetFactoryStorage::instance{};

BuiltInWidgetFactoryStorage::BuiltInWidgetFactoryStorage():
        BuiltInResourceManager({"factories", ".snbt"}){
}

void BuiltInWidgetFactoryStorage::loadFailure(std::exception &e) noexcept {
    static_cast<CompileError&>(e).printErr();
}

BuiltInWidgetFactoryStorage *BuiltInWidgetFactoryStorage::getInstance() {
    return &instance;
}

void BuiltInWidgetFactoryStorage::parseAll() {
    for(auto* f : data) {
        f->parse();
    }
}

WidgetFactory *BuiltInWidgetFactoryStorage::load(const Identifier &loc, const QString &absolutePath) {
    auto* factory = new WidgetFactory(loc.getPath());
    try {
        auto* nbt = NBT::fromStringNbt(absolutePath);
        factory->setSource(nbt);
    } catch (CompileError& e) {
        delete factory;
        throw CompileError(e);
    }
    return factory;
}
