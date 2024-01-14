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
class StringData;
class FloatData;
class BoolData;
class NBT;

class Data {
    friend class ArrayData;
    friend class NBT;
    friend class NBTUtil;
public:
    static const char BYTE = 0;
    static const char SHORT = 1;
    static const char INT = 2;
    static const char LONG = 3;
    static const char FLOAT = 4;
    static const char STRING = 5;
    static const char COMPOUND = 6;
    static const char ARR = 7;
    static const char TAG = 8;
    static const char BOOL = 9;
protected:
    const char type;

public:
    explicit Data(char type);
    IntData* asInt();
    ArrayData* asArray();
    StringData* asString();
    FloatData* asFloat();
    BoolData* asBool();
    NBT* asCompound();
    virtual QString toString() = 0;
    virtual Data* copy() = 0;
    virtual ~Data() = default;
protected:
    virtual void serialize(IByteWriter* writer) = 0;
    virtual void deserialize(IByteReader* reader) = 0;
    static Data* parseData(IByteReader* reader);
};

#endif //LIFERHYTHM_DATA_H
