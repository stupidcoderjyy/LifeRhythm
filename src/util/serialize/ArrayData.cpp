//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "NBT.h"
#include "Preconditions.h"
#include <QStringBuilder>

ArrayData::ArrayData(): Data(ARR) {

}

void ArrayData::serialize(IByteWriter *writer) {
    writer->writeByte(ARR);
    writer->writeInt(data.size());
    for (const auto &item: data) {
        item->serialize(writer);
    }
}

void ArrayData::deserialize(IByteReader *reader) {
    int len = reader->readInt();
    data.reserve(len);
    for (int i = 0 ; i < len ; i ++) {
        data.append(parseData(reader));
    }
}

QString ArrayData::toString() {
    QString res = "[";
    int i = 0;
    while (i < data.size()) {
        res = res % data[i]->toString();
        if (++i == data.size()) {
            break;
        }
        res = res % ',';
    }
    return res % ']';
}

Data *ArrayData::copy() {
    return nullptr;
}

ArrayData::~ArrayData() {
    for (const auto &item: data) {
        delete item;
    }
}

ArrayData *ArrayData::addInt(int val) {
    data.append((new IntData())->setVal(val));
    return this;
}

ArrayData *ArrayData::addArray(ArrayData *val) {
    Preconditions::checkNotNull(val,"ArrayData::addArray");
    data.append(val);
    return this;
}

ArrayData *ArrayData::addTag(NBT *val) {
    Preconditions::checkNotNull(val,"ArrayData::addTag");
    data.append(val);
    return this;
}
