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
    folded = f;
}

void TreeNode::toBytes(IByteWriter *writer) {
    writer->writeBool(folded);
}

void TreeNode::fromBytes(IByteReader *reader) {
    folded = reader->readBool();
}

TreeData::TreeData(): ListData() {
}

void TreeData::toBytes(IByteWriter *writer) {
    writer->writeInt(data.count());
    for (auto* node : data) {
        toBytes(writer, node->cast<TreeNode>());
    }
}

void TreeData::fromBytes(IByteReader *reader) {
    int count = reader->readInt();
    for (int i = 0 ; i < count ; i ++) {
        fromBytes(reader, readElement(reader));
    }
}

void TreeData::foldNode(int idx, bool folded) {
    auto* node = data.at(idx)->cast<TreeNode>();
    if (node->folded == folded) {
        return;
    }
    node->folded = folded;
    onFolded(idx, folded);
}

void TreeData::onFolded(int idx, bool folded) {
    auto* node = data.at(idx)->cast<TreeNode>();
    int count = node->children.count();
    if (!count) {
        return;
    }
    if (folded) {
        fold0(idx);
    } else {
        expand0(idx);
    }
    markChange(idx, data.length() - 1);
}

void TreeData::foldNode(TreeNode *node) {
}

void TreeData::expandNode(TreeNode *node) {
}

TreeNode* TreeData::readElement(IByteReader *reader) {
    return new TreeNode;
}

void TreeData::fold0(int idx) {
    auto* node = data[idx]->cast<TreeNode>();
    foldNode(node);
    int i = idx + 1;
    while (i < data.length()) {
        if (data[i++]->cast<TreeNode>()->parent == node->parent) {
            break;
        }
    }
    data.remove(idx + 1, i - idx - 1);
}

void TreeData::expand0(int idx) {
    auto* end = idx == data.length() - 1 ? nullptr : data[idx + 1];
    int i = idx;
    while (i < data.length()) {
        auto* node = data[i]->cast<TreeNode>();
        if (node == end) {
            end = i == data.length() - 1 ? nullptr : data[i + 1];
            continue;
        }
        i++;
        if (node->folded) {
            continue;
        }
        expandNode(node);
        int count = node->children.length();
        if (count) {
            end = i == data.length() ? nullptr : data[i];
            data.insert(i, count, nullptr);
            memcpy(data.data() + i, node->children.data(), count << 3);
        }
    }
}

void TreeData::toBytes(IByteWriter *writer, TreeNode *node) {
    writer->writeInt(node->children.count());
    for (auto* childNode : node->children) {
        childNode->toBytes(writer);
        toBytes(writer, childNode);
    }
}

void TreeData::fromBytes(IByteReader *reader, TreeNode *parent) {
    int count = reader->readInt();
    parent->children.reserve(count);
    for (int i = 0 ; i < count ; i ++) {
        auto* child = readElement(reader);
        parent->addChildren(child);
        child->fromBytes(reader);
        fromBytes(reader, child);
    }
}
