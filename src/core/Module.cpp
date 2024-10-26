//
// Created by stupid_coder_jyy on 2024/3/30.
//

#include "Module.h"
#include <utility>

USING_NAMESPACE(lr)

Module::Module(QString name): id(std::move(name)) {
}

const QString &Module::getName() {
    return id;
}

void Module::preInit() {
}

void Module::mainInit() {
}

void Module::postInit() {
}
