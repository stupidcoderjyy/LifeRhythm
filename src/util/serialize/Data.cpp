//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "Data.h"
#include "NBT.h"
#include "Preconditions.h"
#include "Error.h"

Data::Data(char type):type(type) {
}

IntData *Data::asInt() {
    Preconditions::checkArgument(type == INT, "Data::asInt", "mismatched type");
    return static_cast<IntData*>(this);
}

ArrayData *Data::asArray() {
    Preconditions::checkArgument(type == ARR, "Data::asArray", "mismatched type");
    return static_cast<ArrayData*>(this);
}

StringData *Data::asString() {
    Preconditions::checkArgument(type == STRING, "Data::asString", "mismatched type");
    return static_cast<StringData*>(this);
}

FloatData *Data::asFloat() {
    Preconditions::checkArgument(type == FLOAT, "Data::asFloat", "mismatched type");
    return static_cast<FloatData*>(this);
}

NBT *Data::asCompound() {
    Preconditions::checkArgument(type == COMPOUND, "Data::asCompound", "mismatched type");
    return static_cast<NBT*>(this);
}

Data *Data::parseData(IByteReader* reader) {
    Data* data = nullptr;
    int type = reader->readByte();
    switch (type) {
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
    Preconditions::checkNotNull(data, "Data::parseData", "unknown data type:" + QString::number(type));
    data->deserialize(reader);
    return data;
}
