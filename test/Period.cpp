//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "Period.h"

#include <utility>

Period::Period(int b, int e, QString info) : begin(b), end(e), info(std::move(info)) {
}

Period::Period(): begin(), end(), info() {
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

WidgetData *PeriodList::readElement(IByteReader *reader) {
    auto* d = new Period();
    d->fromBytes(reader);
    return d;
}

PeriodDataLoader::PeriodDataLoader(QString destPath): ListDataLoader(std::move(destPath)) {
}

WidgetData *PeriodDataLoader::emptyElement() noexcept {
    return new PeriodList;
}