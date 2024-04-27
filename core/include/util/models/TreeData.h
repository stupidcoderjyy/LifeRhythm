//
// Created by stupid_coder_jyy on 2024/2/29.
//

#ifndef LIFERHYTHM_TREEDATA_H
#define LIFERHYTHM_TREEDATA_H

#include "ListData.h"

class TreeData;

class TreeNode : public WidgetData {
    friend class TreeData;
    friend class SelectableTreeData;
    friend class SelectableTreeItem;
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
    virtual void addChildren(TreeData* tree, TreeNode* child);
    virtual void removeChildren(int childIdx);
    virtual void setParent(TreeNode* parent);
    void toBytes(IByteWriter *writer) override;
    void fromBytes(IByteReader *reader) override;
    inline bool isFolded() const;
    inline void setFolded(bool folded);
    inline const QVector<TreeNode *>& getChildren() const;
    inline int getDepth() const;
    inline void setNodeData(WidgetData* data);
    inline WidgetData* nodeData();
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

WidgetData *TreeNode::nodeData() {
    return d;
}

class TreeData : public ListData {
    friend class TreeWidget;
public:
    TreeData();
    void toBytes(IByteWriter *writer) override;
    void fromBytes(IByteReader *reader) override;
    void addNode(TreeNode* node);
    void removeNode(int idx);
    void foldNode(int idx, bool folded);
protected:
    TreeNode *readElement(IByteReader *reader) override;
    virtual int fold0(int idx);
    virtual void expand0(int idx);
private:
    void toBytes(IByteWriter *writer, TreeNode* node);
    void fromBytes(IByteReader* reader, TreeNode* parent);
};


#endif //LIFERHYTHM_TREEDATA_H
