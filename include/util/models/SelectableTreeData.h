//
// Created by stupid_coder_jyy on 2024/3/8.
//

#ifndef LIFERHYTHM_SELECTABLETREEDATA_H
#define LIFERHYTHM_SELECTABLETREEDATA_H

#include "TreeData.h"

class SelectableTreeData : public TreeData {
    Q_OBJECT
    friend class SelectableTreeItem;
protected:
    int selectedIdx;
    TreeNode* selected;
public:
    SelectableTreeData();
    void selectData(int idx);
    void insert(int idx, WidgetData *data) override;
    WidgetData *remove(int idx) override;
signals:
    void sigDataSelected(int pre, int cur);
protected:
    void onNodeFolded(TreeNode *node) override;
};


#endif //LIFERHYTHM_SELECTABLETREEDATA_H
