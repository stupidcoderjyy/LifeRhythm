//
// Created by stupid_coder_jyy on 2024/2/26.
//

#ifndef LIFERHYTHM_TREEWIDGET_H
#define LIFERHYTHM_TREEWIDGET_H

#include "ListWidget.h"
#include "TreeData.h"

class CORE_API TreeItem : public ListItem {
    Q_OBJECT
    friend class TreeWidget;
private:
    bool folded;
public:
    explicit TreeItem(QWidget* parent = nullptr);
    void setFolded(bool f);
    void syncDataToWidget() override;
    void syncWidgetToData() override;
signals:
    void sigItemFold(int idx, bool folded);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void connectModelView() override;
};

class CORE_API TreeWidget : public ListWidget {
public:
    explicit TreeWidget(QWidget* parent = nullptr);
    void setData(TreeData* d);
protected:
    void prepareNewItem(ListItem *item) override;
};


#endif //LIFERHYTHM_TREEWIDGET_H
