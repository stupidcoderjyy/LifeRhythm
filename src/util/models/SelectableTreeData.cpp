//
// Created by stupid_coder_jyy on 2024/3/8.
//

#include "SelectableTreeData.h"

SelectableTreeData::SelectableTreeData(): TreeData(), selected(), selectedIdx(-1) {
}

void SelectableTreeData::selectData(int idx) {
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

void SelectableTreeData::insert(int idx, WidgetData *data) {
    if (selectedIdx >= 0 && idx <= selectedIdx) {
        selectedIdx++;
    }
    TreeData::insert(idx, data);
}

WidgetData *SelectableTreeData::remove(int idx) {
    if (idx == selectedIdx) {
        selectedIdx = -1;
    } else if (idx < selectedIdx) {
        selectedIdx--;
    }
    return TreeData::remove(idx);
}

void SelectableTreeData::onNodeFolded(TreeNode *node) {
    if (selected->parent == node) {
        selected = nullptr;
        selectedIdx = -1;
    }
    TreeData::onNodeFolded(node);
}
