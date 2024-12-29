//
// Created by JYY on 24-10-17.
//

#ifndef NBTSERIALIZABLE_H
#define NBTSERIALIZABLE_H

#include "NBT.h"

BEGIN_LR

class CORE_API NBTSerializable {
public:
    virtual ~NBTSerializable();
    virtual NBT* toNbt();
    virtual void fromNbt(NBT* nbt);
};

END_LR

#endif //NBTSERIALIZABLE_H
