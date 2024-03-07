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
    folded = wData->cast<TreeNode>()->isFolded();
}

void TreeItem::syncWidgetToData() {
    wData->cast<TreeNode>()->setFolded(folded);
}

void TreeItem::setFolded(bool f) {
    if (folded != f) {
        folded = f;
        syncWidgetToData();
        emit sigItemFold(dataIdx, f);
    }
}

TreeWidget::TreeWidget(QWidget *parent):ListWidget(parent) {
}

void TreeWidget::setData(TreeData *d) {
    ListWidget::setData(d);
}

void TreeWidget::prepareNewItem(ListItem *item) {
    ListWidget::prepareNewItem(item);
    auto* w = static_cast<TreeItem*>(item);
    connect(w, &TreeItem::sigItemFold, wData->cast<TreeData>(), &TreeData::onFolded);
}
