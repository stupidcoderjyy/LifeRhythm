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
public:
    TreeNode();
    virtual void addChildren(TreeData* tree, TreeNode* child);
    virtual void removeChildren(int childIdx);
    virtual void setParent(TreeNode* parent);
    bool isFolded() const;
    void setFolded(bool folded);
    void toBytes(IByteWriter *writer) override;
    void fromBytes(IByteReader *reader) override;
    const QVector<TreeNode *>& getChildren() const;
    int getDepth() const;
signals:
    void sigChildCreated(TreeNode* child);
    void sigChildRemoved(int childIdx);
};

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
    virtual void fold0(int idx);
    virtual void expand0(int idx);
private:
    void toBytes(IByteWriter *writer, TreeNode* node);
    void fromBytes(IByteReader* reader, TreeNode* parent);
};


#endif //LIFERHYTHM_TREEDATA_H
