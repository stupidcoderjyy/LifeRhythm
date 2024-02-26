//
// Created by stupid_coder_jyy on 2024/2/26.
//

#ifndef LIFERHYTHM_TREEMODEL_H
#define LIFERHYTHM_TREEMODEL_H

#include "ListModel.h"

class ITreeModel : public IListModel {
public:
    virtual void onFolded(int idx, bool folded) = 0;
};

class TreeWidgetData : public WidgetData {
    friend class TreeModel;
    friend class TreeItem;
    Q_OBJECT
protected:
    bool folded;
    TreeWidgetData* parent;
    QVector<TreeWidgetData*> children;
public:
    TreeWidgetData();
    virtual void addChildren(TreeWidgetData* child);
    virtual void setParent(TreeWidgetData* parent);
    bool isFolded() const;
};

class TreeModel : public ListDataLoader<TreeWidgetData>, public ITreeModel{
public:
    TreeModel(const QString& path, const QString& fileName);
    int length() const override;
    WidgetData *at(int idx) noexcept override;
    void append(WidgetData *data) override;
    void insert(int idx, WidgetData *data) override;
    WidgetData* remove(int idx) override;
    void onFolded(int idx, bool folded) override;
};


#endif //LIFERHYTHM_TREEMODEL_H
