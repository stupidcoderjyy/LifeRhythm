//
// Created by stupid_coder_jyy on 2024/2/29.
//

#include "TreeData.h"

TreeNode::TreeNode(): WidgetData(), parent(), children(), folded(true), depth(), tree() {
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
    auto* c = children[childIdx];
    children.remove(childIdx);
    if (!folded) {
        emit sigChildRemoved(childIdx);
    }
}

void TreeNode::setParent(TreeNode *p) {
    parent = p;
    depth = p->depth + 1;
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

const QVector<TreeNode *> &TreeNode::getChildren() const {
    return children;
}

int TreeNode::getDepth() const {
    return depth;
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
    beginEdit();
    auto* node = data.at(idx)->cast<TreeNode>();
    if (node->folded == folded) {
        return;
    }
    node->folded = folded;
    int count = node->children.count();
    if (!count) {
        return;
    }
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

void TreeData::addNode(TreeNode *node) {
    append(node);
    node->tree = this;
}

void TreeData::removeNode(int idx) {
    foldNode(idx, true);
    remove(idx);
}

void TreeData::onNodeFolded(TreeNode *node) {
}

void TreeData::onNodeExpanded(TreeNode *node) {
}

TreeNode* TreeData::readElement(IByteReader *reader) {
    return new TreeNode;
}

void TreeData::fold0(int i) {
    int removeBegin = i + 1;
    QVector<TreeNode*> nodes;
    nodes << data[i]->cast<TreeNode>();
    while (!nodes.empty()) {
        auto* parent = nodes.takeLast();
        i++;
        while (i < data.length()) {
            auto* child = data[i]->cast<TreeNode>();
            if (child->parent != parent) {
                break;
            }
            if (!child->folded) {
                nodes << child;
                break;
            }
            i++;
        }
    }
    data.remove(removeBegin, i - removeBegin);
}

void TreeData::expand0(int idx) {
    int i = idx;
    QVector<WidgetData*> endNodes;
    endNodes << (idx == data.length() - 1 ? nullptr : data[idx + 1]);
    while (!endNodes.empty()) {
        auto* end = endNodes.last();
        if (!end && i == data.length()) {
            break;
        }
        auto* node = data[i]->cast<TreeNode>();
        if (node == end) {
            endNodes.removeLast();
            continue;
        }
        i++;
        if (node->folded) {
            continue;
        }
        onNodeExpanded(node);
        int count = node->children.length();
        if (count) {
            endNodes << (i == data.length() ? nullptr : data[i]);
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
        parent->addChildren(this, child);
        child->fromBytes(reader);
        fromBytes(reader, child);
    }
}
