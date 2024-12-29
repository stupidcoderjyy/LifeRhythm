//
// Created by JYY on 24-10-17.
//

#ifndef NBTSERIALIZABLE_H
#define NBTSERIALIZABLE_H
#include "NBT.h"

class CORE_API NBTSerializable {
public:
    virtual ~NBTSerializable();
    virtual NBT* toNbt();
    virtual void fromNbt(NBT* nbt);
};

#endif //NBTSERIALIZABLE_H
