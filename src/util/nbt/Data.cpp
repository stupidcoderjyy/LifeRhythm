//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "Data.h"
#include "NBT.h"
#include "Error.h"

Data::Data(char type):type(type) {
}

IntData *Data::asInt() {
    if (type != INT) {
        throwInFunc("mismatched type");
    }
    return static_cast<IntData*>(this);
}

ArrayData *Data::asArray() {
    if (type != ARR) {
        throwInFunc("mismatched type");
    }
    return static_cast<ArrayData*>(this);
}

StringData *Data::asString() {
    if (type != STRING) {
        throwInFunc("mismatched type");
    }
    return static_cast<StringData*>(this);
}

FloatData *Data::asFloat() {
    if (type != FLOAT) {
        throwInFunc("mismatched type");
    }
    return static_cast<FloatData*>(this);
}

BoolData *Data::asBool() {
    if (type != BOOL) {
        throwInFunc("mismatched type");
    }
    return static_cast<BoolData*>(this);
}

NBT *Data::asCompound() {
    if (type != COMPOUND) {
        throwInFunc("mismatched type");
    }
    return static_cast<NBT*>(this);
}

Data *Data::parseData(IByteReader* reader) {
    Data* data = nullptr;
    int type = reader->readByte();
    switch (type) {
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
    }
    if (!data) {
        throwInFunc("unknown data type:" + QString::number(type));
    }
    data->deserialize(reader);
    return data;
}
