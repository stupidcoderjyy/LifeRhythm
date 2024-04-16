//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "Period.h"

#include <utility>

USING_NAMESPACE(lr::log)

Period::Period(int b, int e, QString info): BarData(b, e), info(std::move(info)) {
}

Period::Period(): BarData(), info() {
}

void Period::toBytes(IByteWriter *writer) {
    BarData::toBytes(writer);
    writer->writeString(info);
}

void Period::fromBytes(IByteReader *reader) {
    BarData::fromBytes(reader);
    info = reader->readString();
}