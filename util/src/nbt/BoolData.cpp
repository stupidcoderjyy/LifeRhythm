//
// Created by stupid_coder_jyy on 2024/1/14.
//

#include "BoolData.h"

USING_LR

BoolData::BoolData():Data(BOOL) {

}

void BoolData::serialize(IByteWriter *writer) {
    writer->writeByte(BOOL);
    writer->writeBool(data);
}

void BoolData::deserialize(IByteReader *reader) {
    data = reader->readBool();
}

QString BoolData::toString() {
    return data ? "true" : "false";
}

Data *BoolData::copy() {
    return (new BoolData())->setVal(data);
}
