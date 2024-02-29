//
// Created by stupid_coder_jyy on 2024/2/29.
//

#include "ListData.h"

ListData::ListData(): WidgetData(), changeBegin(), changeEnd(), editing() {
}

void ListData::beginEdit() {
    if (editing) {
        return;
    }
    changeBegin = -1;
    changeEnd = -1;
    editing = true;
}

void ListData::endEdit() {
    if (!editing) {
        return;
    }
    if (changeBegin > 0) {
        emit sigDataChanged();
    }
    editing = false;
}

void ListData::markChange(int min, int max) {
    if (!editing || min > max) {
        return;
    }
    if (changeBegin < 0) {
        changeBegin = min;
        changeEnd = max;
        return;
    }
    if (max < changeBegin || min > changeEnd) {
        emit sigDataChanged();
        changeBegin = min;
        changeEnd = max;
        return;
    }
    changeBegin = qMin(changeBegin, min);
    changeEnd = qMax(changeEnd, max);
}

int ListData::length() const {
    return data.length();
}

WidgetData *ListData::at(int idx) noexcept {
    if (idx < 0 || idx >= data.length()) {
        return nullptr;
    }
    return data.at(idx);
}

void ListData::append(WidgetData *d) {
    int i = data.length();
    data.append(d);
    markChange(i, i);
}

void ListData::insert(int idx, WidgetData *d) {
    data.insert(idx, d);
    markChange(idx + 1, data.length() - 1);
}

WidgetData *ListData::remove(int idx) {
    auto* d = data.at(idx);
    data.remove(idx);
    markChange(idx, data.length() - 1);
    return d;
}

void ListData::toBytes(IByteWriter *writer) {
    writer->writeInt(data.length());
    for (WidgetData* d : data) {
        d->toBytes(writer);
    }
}

void ListData::fromBytes(IByteReader *reader) {
    int l = reader->readInt();
    for (int i = 0 ; i < l ; i ++) {
        append(readElement(reader));
    }
}

int ListData::getChangeBegin() const {
    return changeBegin;
}

int ListData::getChangeEnd() const {
    return changeEnd;
}

WidgetData *ListData::readElement(IByteReader *reader) {
    return nullptr;
}
