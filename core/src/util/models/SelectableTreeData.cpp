//
// Created by stupid_coder_jyy on 2024/3/8.
//

#include "SelectableTreeData.h"

SelectableTreeData::SelectableTreeData(): TreeData(), selectedIdx(-1) {
}

void SelectableTreeData::selectData(int idx) {
    beginEdit();
    select0(idx);
    endEdit();
}

void SelectableTreeData::insert(int idx, WidgetData *data) {
    if (selectedIdx >= 0 && idx <= selectedIdx) {
        selectedIdx++;
    }
    TreeData::insert(idx, data);
}

WidgetData *SelectableTreeData::remove(int idx) {
    if (idx == selectedIdx) {
        auto* node = data[idx]->cast<TreeNode>();
        if (node->parent) {
            int i = idx;
            while (data[--i]->cast<TreeNode>()->parent == node->parent);
            selectData(i);
        } else {
            selectData(-1);
        }
        beginEdit();
    } else if (idx < selectedIdx) {
        selectedIdx--;
    }
    return TreeData::remove(idx);
}

void SelectableTreeData::select0(int idx) {
    if (selectedIdx != idx) {
        int old = selectedIdx;
        selectedIdx = idx;
        if (old >= 0) {
            markChange(old, old);
        }
        if (idx >= 0) {
            markChange(idx, idx);
        }
        emit sigDataSelected(old, idx);
    }
}

int SelectableTreeData::fold0(int i) {
    int removeBegin = i + 1;
    int removeEnd = TreeData::fold0(i);
    if (selectedIdx >= removeBegin) {
        select0(selectedIdx < removeEnd ? removeBegin - 1 : selectedIdx - removeEnd + removeBegin);
    }
    return removeEnd;
}

void SelectableTreeData::expand0(int idx) {
    int i = idx;
    QVector<WidgetData*> endNodes;
    endNodes << (idx == data.length() - 1 ? nullptr : data[idx + 1]);
    while (!endNodes.empty()) {
        auto* end = endNodes.last();
        if (!end && i == data.length()) {
            break;
        }
        auto* parent = data[i]->cast<TreeNode>();
        if (parent == end) {
            endNodes.removeLast();
            continue;
        }
        i++;
        if (parent->folded) {
            continue;
        }
        int count = parent->children.length();
        if (count) {
            endNodes << (i == data.length() ? nullptr : data[i]);
            data.insert(i, count, nullptr);
            memcpy(data.data() + i, parent->children.data(), count << 3);
        }
    }
    if (selectedIdx > idx) {
        select0(selectedIdx + i - idx - 1);
    }
}
