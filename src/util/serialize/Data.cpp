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

NBT *Data::asCompound() {
    Preconditions::checkArgument(type == COMPOUND, "Data::asCompound", "mismatched type");
    return static_cast<NBT*>(this);
}

Data *Data::parseData(IByteReader* reader) {
    Data* data = nullptr;
    char type = reader->readByte();
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
    }
    Preconditions::checkNotNull(data, "Data::parseData", "unknown data type:" + QString::number(type));
    data->deserialize(reader);
    return data;
}
