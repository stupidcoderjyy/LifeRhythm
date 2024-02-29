//
// Created by stupid_coder_jyy on 2024/2/29.
//

#include "SelectableListData.h"

SelectableListData::SelectableListData(): ListData(), selectedIdx(-1) {
}

void SelectableListData::selectData(int idx) {
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
        emit sigDataSelected(old, idx);
    }
}

void SelectableListData::insert(int idx, WidgetData *data) {
    if (selectedIdx >= 0 && idx <= selectedIdx) {
        selectedIdx++;
    }
    ListData::insert(idx, data);
}

WidgetData *SelectableListData::remove(int idx) {
    if (idx == selectedIdx) {
        selectedIdx = -1;
    } else if (idx < selectedIdx) {
        selectedIdx--;
    }
    return ListData::remove(idx);
}