//
// Created by stupid_coder_jyy on 2024/2/25.
//

#include "WidgetData.h"


WidgetData::WidgetData(): QObject() {
}

void WidgetData::toBytes(IByteWriter *writer) {

}

void WidgetData::fromBytes(IByteReader *reader) {

}

WidgetData *IListModel::operator[](int idx) {
    return at(idx);
}

void IListModel::operator<<(WidgetData *d) {
    append(d);
}
