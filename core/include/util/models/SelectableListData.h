//
// Created by stupid_coder_jyy on 2024/2/29.
//

#ifndef LIFERHYTHM_SELECTABLELISTDATA_H
#define LIFERHYTHM_SELECTABLELISTDATA_H

#include "ListData.h"

class SelectableListData : public ListData {
    Q_OBJECT
    friend class SelectableListWidget;
    friend class SelectableListItem;
protected:
    int selectedIdx;
public:
    SelectableListData();
    void selectData(int idx);
    void insert(int idx, WidgetData *data) override;
    WidgetData *remove(int idx) override;
signals:
    void sigDataSelected(int pre, int cur);
};


#endif //LIFERHYTHM_SELECTABLELISTDATA_H
