//
// Created by stupid_coder_jyy on 2023/12/31.
//

#include "StringData.h"

USING_LR

StringData::StringData(): Data(STRING) {
}

void StringData::serialize(IByteWriter *writer) {
    writer->writeByte(STRING);
    writer->writeString(data);
}

void StringData::deserialize(IByteReader *reader) {
    data = reader->readString();
}

QString StringData::toString() {
    return "'" + data + "'";
}

Data *StringData::copy() {
    return (new StringData())->setVal(data);
}
