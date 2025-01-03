//
// Created by JYY on 24-10-15.
//

#include "LongData.h"

USING_LR

LongData::LongData(): Data(LONG) {
}

void LongData::serialize(IByteWriter *writer) {
    writer->writeLong(data);
}

void LongData::deserialize(IByteReader *reader) {
    data = reader->readLong();
}

QString LongData::toString() {
    return QString::number(data) + "L";
}

Data * LongData::copy() {
    return (new LongData())->setVal(data);
}
