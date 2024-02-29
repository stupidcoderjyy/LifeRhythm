//
// Created by stupid_coder_jyy on 2024/2/27.
//

#include "SelectableListWidget.h"

SelectableListModel::SelectableListModel(const QString &path, const QString &fileName):
        ListModel(path, fileName), selectedIdx(-1) {
}

void SelectableListModel::selectData(int idx) {
    if (selectedIdx != idx) {
        int old = selectedIdx;
        selectedIdx = idx;
        if (old >= 0) {
            emit sigDataChanged(old, old);
        }
        if (idx >= 0) {
            emit sigDataChanged(idx, idx);
        }
        emit sigDataSelected(old, idx);
    }
}

void SelectableListModel::insert(int idx, WidgetData *data) {
    if (selectedIdx >= 0 && idx <= selectedIdx) {
        selectedIdx++;
    }
    ListModel::insert(idx, data);
}

WidgetData *SelectableListModel::remove(int idx) {
    if (idx == selectedIdx) {
        selectedIdx = -1;
    } else if (idx < selectedIdx) {
        selectedIdx--;
    }
    return ListModel::remove(idx);
}

SelectableListItem::SelectableListItem(QWidget *parent):
        ListItem(parent), selected(), model() {
}

void SelectableListItem::mousePressEvent(QMouseEvent *event) {
    ListItem::mousePressEvent(event);
    model->selectData(dataIdx);
}

void SelectableListItem::updateItemAfterSelecting(bool s) {
}

void SelectableListItem::syncDataToWidget() {
    bool s = model->selectedIdx == dataIdx;
    if (selected == s) {
        return;
    }
    updateItemAfterSelecting(s);
    selected = s;
}

void SelectableListItem::setModel(SelectableListModel *m) {
    model = m;
}

SelectableListWidget::SelectableListWidget(QWidget *parent): ListWidget(parent) {
}

void SelectableListWidget::setModel(SelectableListModel *model) {
    ListWidget::setModel(model);
}

SelectableListItem *SelectableListWidget::createRowItem() {
    return new SelectableListItem;
}

void SelectableListWidget::prepareNewItem(ListItem *item) {
    static_cast<SelectableListItem*>(item)->setModel(static_cast<SelectableListModel*>(model));
    ListWidget::prepareNewItem(item);
}
