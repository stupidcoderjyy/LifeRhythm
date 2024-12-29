//
// Created by stupid_coder_jyy on 2024/2/29.
//

#include "ListData.h"

ListData::ListData(): WidgetData(), selectedIdx(-1), changeBegin(), changeEnd(), editing() {
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
    if (changeBegin >= 0) {
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
    if (max < changeBegin - 1 || min > changeEnd + 1) {
        emit sigDataChanged();
        changeBegin = min;
        changeEnd = max;
        return;
    }
    changeBegin = qMin(changeBegin, min);
    changeEnd = qMax(changeEnd, max);
}

void ListData::markAll() {
    beginEdit();
    markChange(0, data.length() - 1);
    endEdit();
}

void ListData::selectData(int idx) {
    if (selectedIdx != idx) {
        beginEdit();
        int old = selectedIdx;
        selectedIdx = idx;
        if (old >= 0) {
            markChange(old, old);
        }
        if (idx >= 0) {
            markChange(idx, idx);
        }
        endEdit();
        if (idx >= 0) {
            emit sigDataSelected(old, idx);
        }
    }
}

void ListData::append(WidgetData *d) {
    int i = data.length();
    data.append(d);
    markChange(i, i);
}

void ListData::insert(int idx, WidgetData *d) {
    if (selectedIdx >= 0 && idx <= selectedIdx) {
        selectedIdx++;
    }
    data.insert(idx, d);
    markChange(idx, data.length() - 1);
}

WidgetData *ListData::remove(int idx) {
    if (idx == selectedIdx) {
        selectedIdx = -1;
    } else if (idx < selectedIdx) {
        selectedIdx--;
    }
    auto* d = data.takeAt(idx);
    markChange(idx, data.length());
    return d;
}

void ListData::clear() {
    data.clear();
    markChange(0, data.length());
    editing = false;
}
