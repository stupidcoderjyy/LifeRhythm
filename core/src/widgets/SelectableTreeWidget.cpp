//
// Created by stupid_coder_jyy on 2024/3/8.
//

#include "SelectableTreeWidget.h"

SelectableTreeItem::SelectableTreeItem(QWidget *parent): TreeItem(parent) {
}

void SelectableTreeItem::syncDataToWidget() {
    TreeItem::syncDataToWidget();
    bool s = treeModel->selectedIdx == dataIdx;
    if (selected == s) {
        return;
    }
    selected = s;
}

void SelectableTreeItem::mousePressEvent(QMouseEvent *event) {
    ListItem::mousePressEvent(event);
    if (wData) {
        treeModel->selectData(dataIdx);
    }
}

SelectableTreeWidget::SelectableTreeWidget(QWidget *parent): TreeWidget(parent) {
}

SelectableTreeItem *SelectableTreeWidget::createRowItem() {
    return new SelectableTreeItem;
}

void SelectableTreeWidget::prepareNewItem(ListItem *item) {
    static_cast<SelectableTreeItem*>(item)->treeModel = wData->cast<SelectableTreeData>();
    TreeWidget::prepareNewItem(item);
}
