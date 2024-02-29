//
// Created by stupid_coder_jyy on 2024/2/29.
//

#include "TreeData.h"

TreeNode::TreeNode(): WidgetData(), parent(), children(), folded(true) {
}

void TreeNode::addChildren(TreeNode *c) {
    children << c;
    c->setParent(this);
}

void TreeNode::setParent(TreeNode *p) {
    parent = p;
}

bool TreeNode::isFolded() const {
    return folded;
}

void TreeNode::setFolded(bool f) {
    TreeNode::folded = f;
}

TreeData::TreeData(): ListData() {
}

void TreeData::onFolded(int idx, bool folded) {
    auto* item = data.at(idx)->cast<TreeNode>();
    int count = item->children.count();
    if (!count) {
        return;
    }
    idx++;
    if (!folded) {
        data.insert(idx, count, nullptr);
        memcpy(data.data() + idx, item->children.data(), count << 3);
    } else {
        data.remove(idx, count);
    }
    markChange(idx - 1, data.length() - 1);
}
