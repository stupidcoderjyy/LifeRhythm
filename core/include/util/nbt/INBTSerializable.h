//
// Created by JYY on 24-10-17.
//

#ifndef INBTSERIALIZABLE_H
#define INBTSERIALIZABLE_H
#include "NBT.h"

class CORE_API NBTSerializable {
public:
    virtual ~NBTSerializable() = default;
    virtual NBT* toNbt() = 0;
    virtual void fromNbt(NBT* nbt) = 0;
};

#endif //INBTSERIALIZABLE_H
