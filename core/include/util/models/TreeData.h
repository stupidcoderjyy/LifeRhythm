//
// Created by stupid_coder_jyy on 2024/2/29.
//

#ifndef LIFERHYTHM_TREEDATA_H
#define LIFERHYTHM_TREEDATA_H

#include "ListData.h"
#include "Plugin.h"

class TreeData;
class CORE_API TreeNode final : public WidgetData {
    friend class TreeData;
    friend class TreeItem;
    Q_OBJECT
protected:
    int depth;
    bool folded;
    TreeNode* parent;
    QVector<TreeNode*> children;
    TreeData* tree;
    WidgetData* d;
public:
    explicit TreeNode(WidgetData* d = nullptr);
    void addChildren(TreeData* tree, TreeNode* child);
    void removeChildren(int childIdx);
    void setParent(TreeNode* parent);
    inline bool isFolded() const;
    inline void setFolded(bool folded);
    inline const QVector<TreeNode *>& getChildren() const;
    inline int getDepth() const;
    inline void setNodeData(WidgetData* data);
    inline WidgetData* nodeData() const;
signals:
    void sigChildCreated(TreeNode* child);
    void sigChildRemoved(int childIdx);
};

inline bool TreeNode::isFolded() const {
    return folded;
}

inline void TreeNode::setFolded(bool f) {
    folded = f;
}

inline const QVector<TreeNode *> &TreeNode::getChildren() const {
    return children;
}

inline int TreeNode::getDepth() const {
    return depth;
}

inline void TreeNode::setNodeData(WidgetData *data) {
    d = data;
}

WidgetData *TreeNode::nodeData() const {
    return d;
}

class CORE_API TreeData final : public ListData {
    friend class TreeWidget;
public:
    void addNode(TreeNode* node);
    void removeNode(int idx);
    void foldNode(int idx, bool folded);
    WidgetData* remove(int idx) override;
protected:
    int fold0(int idx);
    void expand0(int idx);
};


#endif //LIFERHYTHM_TREEDATA_H
