//
// Created by stupid_coder_jyy on 2024/3/8.
//

#ifndef LIFERHYTHM_SELECTABLETREEWIDGET_H
#define LIFERHYTHM_SELECTABLETREEWIDGET_H

#include "TreeWidget.h"
#include "SelectableTreeData.h"

class SelectableTreeItem : public TreeItem {
    friend class SelectableTreeWidget;
protected:
    SelectableTreeData* treeModel;
    bool selected;
public:
    explicit SelectableTreeItem(QWidget* parent = nullptr);
    void syncDataToWidget() override;
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class SelectableTreeWidget : public TreeWidget{
public:
    explicit SelectableTreeWidget(QWidget* parent = nullptr);
protected:
    SelectableTreeItem *createRowItem() override;
    void prepareNewItem(ListItem *item) override;
};


#endif //LIFERHYTHM_SELECTABLETREEWIDGET_H
