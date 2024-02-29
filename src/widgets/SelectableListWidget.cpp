//
// Created by stupid_coder_jyy on 2024/2/27.
//

#include "SelectableListWidget.h"

SelectableListItem::SelectableListItem(QWidget *parent):
        ListItem(parent), selected(), parentList() {
}

void SelectableListItem::mousePressEvent(QMouseEvent *event) {
    ListItem::mousePressEvent(event);
    parentList->selectData(dataIdx);
}

void SelectableListItem::updateItemAfterSelecting(bool s) {
}

void SelectableListItem::syncDataToWidget() {
    bool s = parentList->selectedIdx == dataIdx;
    if (selected == s) {
        return;
    }
    updateItemAfterSelecting(s);
    selected = s;
}

void SelectableListItem::setParentList(SelectableListData *l) {
    parentList = l;
}

SelectableListWidget::SelectableListWidget(QWidget *parent): ListWidget(parent) {
}

void SelectableListWidget::setData(SelectableListData *d) {
    ListWidget::setData(d);
}

SelectableListItem *SelectableListWidget::createRowItem() {
    return new SelectableListItem;
}

void SelectableListWidget::prepareNewItem(ListItem *item) {
    static_cast<SelectableListItem *>(item)->setParentList(wData->cast<SelectableListData>());
    ListWidget::prepareNewItem(item);
}
