//
// Created by stupid_coder_jyy on 2024/4/2.
//

#include "NestedListData.h"
#include "MemUtil.h"

NestedListNode::NestedListNode(): WidgetData(), row(-1) {
}

NestedListData::NestedListData(): WidgetData(), columnChangeBegin(), columnChangeEnd(), rowChangeBegin(),
          rowChangeEnd(-1), data(), editType(None), editing() {
}

NestedListData::~NestedListData() {
    DELETE_NESTED_LIST(data)
}

void NestedListData::removeRow(int idx) {
    DELETE_LIST_P(data.takeAt(idx))
    markRow(idx, data.length());
}

void NestedListData::insertRow(int idx, int count) {
    data.insert(idx, count, nullptr);
    for (int i = 0 ; i < count ; i ++) {
        data[idx++] = new QVector<NestedListNode*>();
    }
    markRow(idx, data.length() - 1);
}

void NestedListData::makeRow(int count) {
    int len = data.length();
    for (int i = 0 ; i < count ; i ++) {
        data << new QVector<NestedListNode*>();
    }
    markRow(len, len + count - 1);
}

void NestedListData::append(int row, NestedListNode *d) {
    if (row >= data.length()) {
        makeRow(data.length() - row + 1);
    }
    auto* c = data[row];
    int len = c->length();
    c->append(d);
    initNode(d, row);
    markColumn(row, len, len);
}

void NestedListData::insert(int row, int n, NestedListNode *d) {
    if (row >= data.length()) {
        makeRow(data.length() - row);
    }
    auto* c = data[row];
    initNode(d, row);
    c->insert(n, d);
    markColumn(row, n, c->length() - 1);
}

NestedListNode *NestedListData::remove(int row, int n) {
    if (row >= data.length()) {
        return nullptr;
    }
    auto* d = data[row]->takeAt(n);
    d->row = -1;
    markColumn(row, n, data[row]->length());
    return d;
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
        columnChangeBegin = -1;
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

void NestedListData::markAll() {
    beginEdit();
    markRow(0, data.length() - 1);
    for (int i = 0 ; i < data.length() ; i ++) {
        markColumn(i, 0, data[i]->length() - 1);
    }
    endEdit();
}

void NestedListData::endEdit() {
    if (!editing || editType == None) {
        return;
    }
    emit sigDataChanged();
    editing = false;
}

void NestedListData::initNode(NestedListNode *d, int row) {
    d->row = row;
    connect(d, &NestedListNode::sigRowChanged, this, [this, d](int old, int cur){
        beginEdit();
        if (old >= 0) {
            remove(old, data[old]->indexOf(d));
        }
        append(cur, d);
        endEdit();
    }, Qt::UniqueConnection);
}
