//
// Created by stupid_coder_jyy on 2024/1/1.
//

#include "FloatData.h"

FloatData::FloatData(): Data(FLOAT) {

}

FloatData *FloatData::setVal(float val) {
    data = val;
    return this;
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
