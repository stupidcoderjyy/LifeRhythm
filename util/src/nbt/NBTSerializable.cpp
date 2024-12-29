//
// Created by JYY on 24-10-26.
//

#include "NBTSerializable.h"

USING_LR

NBTSerializable::~NBTSerializable() = default;

NBT * NBTSerializable::toNbt() {
    return nullptr;
}

void NBTSerializable::fromNbt(NBT *nbt) {
}
