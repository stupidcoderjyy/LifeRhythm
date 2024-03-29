//
// Created by stupid_coder_jyy on 2024/2/27.
//

#ifndef LIFERHYTHM_SELECTABLELISTWIDGET_H
#define LIFERHYTHM_SELECTABLELISTWIDGET_H

#include "ListWidget.h"
#include "SelectableListData.h"

class SelectableListItem : public ListItem {
    friend class SelectableListWidget;
protected:
    SelectableListData* parentList;
    bool selected;
public:
    explicit SelectableListItem(QWidget* parent = nullptr);
    void syncDataToWidget() override;
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class SelectableListWidget : public ListWidget{
public:
    explicit SelectableListWidget(QWidget* parent = nullptr);
    void setData(SelectableListData* d);
protected:
    SelectableListItem *createRowItem() override;
    void prepareNewItem(ListItem *item) override;
};


#endif //LIFERHYTHM_SELECTABLELISTWIDGET_H
