//
// Created by stupid_coder_jyy on 2024/1/1.
//

#include "FloatData.h"

USING_LR

FloatData::FloatData(): Data(FLOAT) {

}

void FloatData::serialize(IByteWriter *writer) {
    writer->writeByte(FLOAT);
    writer->writeDouble(data);
}

void FloatData::deserialize(IByteReader *reader) {
    data = reader->readFloat();
}

QString FloatData::toString() {
    return QString::number(data);
}

Data *FloatData::copy() {
    return (new FloatData())->setVal(data);
}
