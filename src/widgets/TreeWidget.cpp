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
    folded = data->cast<TreeWidgetData>()->folded;
}

void TreeItem::syncWidgetToData() {
    data->cast<TreeWidgetData>()->folded = folded;
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

void TreeWidget::setModel(ITreeModel *model) {
    ListWidget::setModel(model);
}

void TreeWidget::prepareNewItem(ListItem *item) {
    ListWidget::prepareNewItem(item);
    auto* w = static_cast<TreeItem*>(item);
    connect(w, &TreeItem::sigItemFold, static_cast<TreeModel*>(model), &ITreeModel::onFolded);
}
