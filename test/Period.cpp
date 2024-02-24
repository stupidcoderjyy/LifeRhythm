//
// Created by stupid_coder_jyy on 2024/2/24.
//

#include "Period.h"

#include <utility>

Period::Period():RangeWidgetData(),info() {
}

Period::Period(int b, int e, QString i) {
    begin = b;
    end = e;
    info = std::move(i);
}

void Period::toBytes(IByteWriter *writer) {
    writer->writeInt(begin);
    writer->writeInt(end);
    writer->writeString(info);
}

void Period::fromBytes(IByteReader *reader) {
    begin = reader->readInt();
    end = reader->readInt();
    info = reader->readString();
}

const QString &Period::getInfo() const {
    return info;
}

void Period::setInfo(const QString &i) {
    info = i;
}
