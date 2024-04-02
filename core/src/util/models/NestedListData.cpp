//
// Created by stupid_coder_jyy on 2024/4/2.
//

#include "NestedListData.h"

#define FREE_LIST(pList) \
    for (auto* d : *pList) {\
        delete d;\
    }\
    delete pList;


NestedListData::NestedListData(): WidgetData(), columnChangeBegin(), columnChangeEnd(), rowChangeBegin(),
          rowChangeEnd(-1), data(), editType(None), editing() {
}

NestedListData::~NestedListData() {
    for (auto* list : data) {
        FREE_LIST(list)
    }
}

void NestedListData::removeRow(int idx) {
    FREE_LIST(data.takeAt(idx))
    markRow(idx, data.length());
}

void NestedListData::insertRow(int idx, int count) {
    data.insert(idx, count, nullptr);
    for (int i = 0 ; i < count ; i ++) {
        data[idx++] = new QVector<WidgetData*>();
    }
    markRow(idx, data.length() - 1);
}

void NestedListData::insertRow(int idx) {
    insertRow(idx, 1);
}

void NestedListData::appendRow(int count) {
    int len = data.length();
    for (int i = 0 ; i < count ; i ++) {
        data << new QVector<WidgetData*>();
    }
    markRow(len, len + count - 1);
}

void NestedListData::appendRow() {
    appendRow(1);
}

void NestedListData::append(int row, WidgetData *d) {
    auto* c = data[row];
    int len = c->length();
    c->append(d);
    markColumn(row, len, len);
}

void NestedListData::insert(int row, int n, WidgetData *d) {
    auto* c = data[row];
    c->insert(n, d);
    markColumn(row, n, c->length() - 1);
}

WidgetData *NestedListData::remove(int row, int n) {
    auto* d = data[row]->takeAt(n);
    markColumn(row, n, data[row]->length());
    return d;
}

QVector<WidgetData *> *NestedListData::getRow(int row) {
    return data[row];
}

WidgetData *NestedListData::at(int row, int n) {
    return data[row]->at(n);
}

void NestedListData::beginEdit() {
    editType = None;
    rowChangeBegin = -1;
    rowChangeEnd = -1;
    columnChangeBegin = -1;
    columnChangeEnd = -1;
    editing = true;
}

void NestedListData::markRow(int min, int max) {
    if (!editing || min > max) {
        return;
    }
    if (editType == Column) {
        emit sigDataChanged();
    }
    editType = Row;
    if (rowChangeBegin < 0) {
        rowChangeBegin = min;
        rowChangeEnd = max;
        return;
    }
    if (max < rowChangeBegin - 1 || min > rowChangeEnd + 1) {
        emit sigDataChanged();
        rowChangeBegin = min;
        rowChangeBegin = max;
        return;
    }
    rowChangeBegin = qMin(rowChangeBegin, min);
    rowChangeEnd = qMax(rowChangeEnd, max);
}

void NestedListData::markColumn(int row, int min, int max) {
    if (!editing || min > max) {
        return;
    }
    if (editType == Row) {
        emit sigDataChanged();
    }
    editType = Column;
    if (columnChangeBegin < 0) {
        columnChangeBegin = min;
        columnChangeEnd = max;
        dirtyRow = row;
        return;
    }
    if (max < columnChangeBegin - 1 || min > columnChangeEnd + 1 || row != dirtyRow) {
        emit sigDataChanged();
        columnChangeBegin = min;
        columnChangeBegin = max;
        dirtyRow = row;
        return;
    }
    columnChangeBegin = qMin(columnChangeBegin, min);
    columnChangeEnd = qMax(columnChangeEnd, max);
}

void NestedListData::endEdit() {
    if (!editing || editType == None) {
        return;
    }
    emit sigDataChanged();
    editing = false;
}

int NestedListData::getColumnChangeBegin() const {
    return columnChangeBegin;
}

int NestedListData::getColumnChangeEnd() const {
    return columnChangeEnd;
}

int NestedListData::getDirtyRow() const {
    return dirtyRow;
}

int NestedListData::getRowChangeBegin() const {
    return rowChangeBegin;
}

int NestedListData::getRowChangeEnd() const {
    return rowChangeEnd;
}

NestedListData::EditType NestedListData::getEditType() const {
    return editType;
}
