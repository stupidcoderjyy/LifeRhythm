//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_DATA_H
#define LIFERHYTHM_DATA_H

#include <QString>
#include "IByteWriter.h"
#include "IByteReader.h"

class ArrayData;
class IntData;
class NBT;

class Data {
    friend class ArrayData;
    friend class NBT;
    friend class NBTUtil;
public:
    static const char BYTE = 0x01;
    static const char SHORT = 0x02;
    static const char INT = 0x04;
    static const char LONG = 0x08;
    static const char DOUBLE = 0x18;
    static const char STRING = 0x20;
    static const char COMPOUND = 0x30;
    static const char ARR = 0x40;
    static const char TAG = 0x50;
protected:
    const char type;

public:
    explicit Data(char type);
    IntData* asInt();
    ArrayData* asArray();
    NBT* asCompound();
    virtual QString toString() = 0;
    virtual Data* copy() = 0;
protected:
    virtual void serialize(IByteWriter* writer) = 0;
    virtual void deserialize(IByteReader* reader) = 0;
    static Data* parseData(IByteReader* reader);
    virtual ~Data() = default;
};

#endif //LIFERHYTHM_DATA_H
