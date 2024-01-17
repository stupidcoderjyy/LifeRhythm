//
// Created by stupid_coder_jyy on 2023/12/28.
//
#include "IntData.h"

IntData::IntData() : Data(INT){}

void IntData::serialize(IByteWriter *writer) {
    writer->writeByte(INT);
    writer->writeInt(data);
}

void IntData::deserialize(IByteReader *reader) {
    data = reader->readInt();
}

QString IntData::toString() {
    return QString::number(data);
}

Data *IntData::copy() {
    return (new IntData())->setVal(data);
}

