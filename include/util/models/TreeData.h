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
    void toBytes(IByteWriter *writer) override;
    void fromBytes(IByteReader *reader) override;
};

class TreeData : public ListData {
    friend class TreeWidget;
public:
    TreeData();
    void toBytes(IByteWriter *writer) override;
    void fromBytes(IByteReader *reader) override;
    void foldNode(int idx, bool folded = true);
protected:
    virtual void foldNode(TreeNode* node);
    virtual void expandNode(TreeNode* node);
    TreeNode *readElement(IByteReader *reader) override;
private:
    void onFolded(int idx, bool folded);
    void fold0(int idx);
    void expand0(int idx);
    void toBytes(IByteWriter *writer, TreeNode* node);
    void fromBytes(IByteReader* reader, TreeNode* parent);
};


#endif //LIFERHYTHM_TREEDATA_H
