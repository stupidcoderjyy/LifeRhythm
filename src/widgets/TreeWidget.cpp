//
// Created by stupid_coder_jyy on 2024/2/26.
//

#include "TreeWidget.h"

TreeItem::TreeItem(QWidget *parent):ListItem(parent), folded(true) {
}

void TreeItem::mouseDoubleClickEvent(QMouseEvent *event) {
    setFolded(!folded);
}

void TreeItem::syncDataToWidget() {
    folded = wData ? wData->cast<TreeNode>()->isFolded() : true;
}

void TreeItem::syncWidgetToData() {
    if (wData) {
        wData->cast<TreeNode>()->setFolded(folded);
    }
}

void TreeItem::setFolded(bool f) {
    if (folded != f) {
        folded = f;
        syncWidgetToData();
        emit sigItemFold(dataIdx, f);
    }
}

void TreeItem::connectModelView() {
    auto* node = wData->cast<TreeNode>();
    dc << connect(node, &TreeNode::sigChildCreated, this, [this, node](TreeNode* child){
        auto* tree = node->tree;
        tree->beginEdit();
        tree->insert(dataIdx + node->children.length(), child);
        tree->endEdit();
    });
    dc << connect(node, &TreeNode::sigChildRemoved, this, [this, node](int childIdx) {
        auto* tree = node->tree;
        tree->beginEdit();
        tree->remove(dataIdx + childIdx + 1);
        tree->endEdit();
    });
}

TreeWidget::TreeWidget(QWidget *parent):ListWidget(parent) {
}

void TreeWidget::setData(TreeData *d) {
    ListWidget::setData(d);
}

void TreeWidget::prepareNewItem(ListItem *item) {
    ListWidget::prepareNewItem(item);
    auto* w = static_cast<TreeItem*>(item);
    connect(w, &TreeItem::sigItemFold, wData->cast<TreeData>(), &TreeData::foldNode);
}
