//
// Created by stupid_coder_jyy on 2023/12/31.
//

#include "StringData.h"

StringData::StringData(): Data(STRING) {
}

StringData *StringData::setVal(const QString &val) {
    data = val;
    return this;
}

void StringData::serialize(IByteWriter *writer) {
    writer->writeByte(STRING);
    writer->writeString(data);
}

void StringData::deserialize(IByteReader *reader) {
    data = reader->readString();
}

QString StringData::toString() {
    return data;
}

Data *StringData::copy() {
    return (new StringData())->setVal(data);
}
