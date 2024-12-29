//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "Data.h"
#include "NBT.h"
#include "Error.h"
#include "LongData.h"

Data::Data(char type):type(type) {
}

IntData *Data::asInt() {
    return static_cast<IntData*>(this);
}

ArrayData *Data::asArray() {
    return static_cast<ArrayData*>(this);
}

StringData *Data::asString() {
    return static_cast<StringData*>(this);
}

FloatData *Data::asFloat() {
    return static_cast<FloatData*>(this);
}

BoolData *Data::asBool() {
    return static_cast<BoolData*>(this);
}

LongData *Data::asLong() {
    return static_cast<LongData*>(this);
}

NBT *Data::asCompound() {
    return static_cast<NBT*>(this);
}

Data *Data::parseData(IByteReader* reader) {
    Data* data = nullptr;
    switch (int type = reader->readByte()) {
        case BOOL: {
            data = new BoolData();
            break;
        }
        case FLOAT: {
            data = new FloatData();
            break;
        }
        case INT: {
            data = new IntData();
            break;
        }
        case ARR: {
            data = new ArrayData();
            break;
        }
        case COMPOUND: {
            data = new NBT();
            break;
        }
        case STRING: {
            data = new StringData();
            break;
        }
        case LONG: {
            data = new LongData();
            break;
        }
        default:
            throwInFunc("unknown data type:" + QString::number(type));
    }
    data->deserialize(reader);
    return data;
}
