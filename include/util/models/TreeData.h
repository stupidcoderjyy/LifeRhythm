//
// Created by stupid_coder_jyy on 2024/2/29.
//

#ifndef LIFERHYTHM_TREEDATA_H
#define LIFERHYTHM_TREEDATA_H

#include "ListData.h"

class TreeData;

class TreeNode : public WidgetData {
    friend class TreeData;
    Q_OBJECT
protected:
    bool folded;
    TreeNode* parent;
    QVector<TreeNode*> children;
public:
    TreeNode();
    virtual void addChildren(TreeNode* child);
    virtual void setParent(TreeNode* parent);
    bool isFolded() const;
    void setFolded(bool folded);
};

class TreeData : public ListData {
    friend class TreeWidget;
public:
    TreeData();
protected:
    virtual void onFolded(int idx, bool folded);
};


#endif //LIFERHYTHM_TREEDATA_H
