//
// Created by stupid_coder_jyy on 2024/2/29.
//

#include "TreeData.h"

USING_LR

TreeNode::TreeNode(WidgetData *d): WidgetData(), depth(), folded(true), parent(), tree(), d(d) {
}

void TreeNode::addChildren(TreeData* t, TreeNode *c) {
    children << c;
    tree = t;
    c->setParent(this);
    if (!folded) {
        emit sigChildCreated(c);
    }
}

void TreeNode::removeChildren(int childIdx) {
    children.remove(childIdx);
    if (!folded) {
        emit sigChildRemoved(childIdx);
    }
}

void TreeNode::setParent(TreeNode *p) {
    parent = p;
    depth = p->depth + 1;
}

void TreeData::foldNode(int idx, bool folded) {
    auto* node = data.at(idx)->cast<TreeNode>();
    if (node->folded == folded) {
        return;
    }
    node->folded = folded;
    int count = node->children.count();
    if (!count) {
        return;
    }
    beginEdit();
    if (folded) {
        int oldLen = data.length();
        fold0(idx);
        markChange(idx, oldLen - 1);
    } else {
        expand0(idx);
        markChange(idx, data.length() - 1);
    }
    endEdit();
}

WidgetData * TreeData::remove(int idx) {
    if (idx == selectedIdx) {
        if (auto* node = data[idx]->cast<TreeNode>(); node->parent) {
            int i = idx;
            while (data[--i]->cast<TreeNode>()->parent == node->parent) {}
            selectData(i);
        } else {
            selectData(-1);
        }
        beginEdit();
    } else if (idx < selectedIdx) {
        selectedIdx--;
    }
    auto* d = data.takeAt(idx);
    markChange(idx, data.length());
    return d;
}

void TreeData::addNode(TreeNode *node) {
    append(node);
    node->tree = this;
}

void TreeData::removeNode(int idx) {
    foldNode(idx, true);
    remove(idx);
}

int TreeData::fold0(int i) {
    const int removeBegin = i + 1;
    QVector<TreeNode*> nodes;
    nodes << data[i++]->cast<TreeNode>();
    while (!nodes.empty()) {
        auto* parent = nodes.last();
        while (true) {
            if (i == data.length()) {
                nodes.removeLast();
                break;
            }
            auto* child = data[i]->cast<TreeNode>();
            if (child->parent != parent) {
                nodes.removeLast();
                break;
            }
            i++;
            if (!child->folded) {
                nodes << child;
                break;
            }
        }
    }
    data.remove(removeBegin, i - removeBegin);
    const int removeEnd = i;
    if (selectedIdx >= removeBegin) {
        selectData(selectedIdx < removeEnd ? removeBegin - 1 : selectedIdx - removeEnd + removeBegin);
    }
    return removeEnd;
}

void TreeData::expand0(const int idx) {
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
        if (int count = parent->children.length()) {
            endNodes << (i == data.length() ? nullptr : data[i]);
            data.insert(i, count, nullptr);
            memcpy(data.data() + i, parent->children.data(), count << 3);
        }
    }
    if (selectedIdx > idx) {
        selectData(selectedIdx + i - idx - 1);
    }
}