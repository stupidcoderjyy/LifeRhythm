//
// Created by stupid_coder_jyy on 2024/2/27.
//

#ifndef LIFERHYTHM_SELECTABLELISTWIDGET_H
#define LIFERHYTHM_SELECTABLELISTWIDGET_H

#include "ListWidget.h"

class SelectableListModel : public ListModel {
    Q_OBJECT
    friend class SelectableListWidget;
    friend class SelectableListItem;
protected:
    int selectedIdx;
public:
    SelectableListModel(const QString &path, const QString &fileName);
    void selectData(int idx);
    void insert(int idx, WidgetData *data) override;
    WidgetData *remove(int idx) override;
signals:
    void sigDataSelected(int pre, int cur);
};

class SelectableListItem : public ListItem {
    friend class SelectableListWidget;
protected:
    SelectableListModel* model;
    bool selected;
public:
    explicit SelectableListItem(QWidget* parent = nullptr);
    virtual void updateItemAfterSelecting(bool selected);
    void syncDataToWidget() override;
    void setModel(SelectableListModel* model);
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class SelectableListWidget : public ListWidget{
public:
    explicit SelectableListWidget(QWidget* parent = nullptr);
    void setModel(SelectableListModel* model);
protected:
    SelectableListItem *createRowItem() override;
    void prepareNewItem(ListItem *item) override;
};


#endif //LIFERHYTHM_SELECTABLELISTWIDGET_H
