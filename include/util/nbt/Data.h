//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_DATA_H
#define LIFERHYTHM_DATA_H

#include "IByteWriter.h"
#include "IByteReader.h"
#include "Plugin.h"

class ArrayData;
class IntData;
class StringData;
class FloatData;
class BoolData;
class LongData;
class NBT;

class CORE_API Data {
    friend class ArrayData;
    friend class NBT;
    friend class NBTUtil;
public:
    const char type;
    static const char BYTE = 0;
    static const char SHORT = 1;
    static const char INT = 2;
    static const char LONG = 3;
    static const char FLOAT = 4;
    static const char STRING = 5;
    static const char COMPOUND = 6;
    static const char TAG = 7;
    static const char BOOL = 8;
    static const char ARR = 9;

public:
    static Data* parseData(IByteReader* reader);
    explicit Data(char type);
    IntData* asInt();
    ArrayData* asArray();
    StringData* asString();
    FloatData* asFloat();
    BoolData* asBool();
    NBT* asCompound();
    LongData* asLong();
    virtual void serialize(IByteWriter* writer) = 0;
    virtual void deserialize(IByteReader* reader) = 0;
    virtual QString toString() = 0;
    virtual Data* copy() = 0;
    virtual ~Data() = default;
};

#endif //LIFERHYTHM_DATA_H
