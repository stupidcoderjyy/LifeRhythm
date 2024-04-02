//
// Created by stupid_coder_jyy on 2024/4/2.
//

#ifndef LIFERHYTHM_NESTEDLISTDATA_H
#define LIFERHYTHM_NESTEDLISTDATA_H

#include "ListData.h"

class NestedListData : public WidgetData {
public:
    enum EditType {
        None,
        Row,        //行的增加和删除
        Column,     //某一行内元素的增加和删除
    };
private:
    int columnChangeBegin;
    int columnChangeEnd;
    int dirtyRow;              //使得在进行Column类型更改时，知道当前是第几行
    int rowChangeBegin;
    int rowChangeEnd;
    bool editing;
    EditType editType;
    QVector<QVector<WidgetData*>*> data;
public:
    NestedListData();
    ~NestedListData() override;
    virtual void removeRow(int idx);
    virtual void insertRow(int idx, int count);
    void insertRow(int idx);
    virtual void appendRow(int count);
    void appendRow();
    virtual void append(int row, WidgetData* d);
    virtual void insert(int row, int n, WidgetData* d);
    virtual WidgetData* remove(int row, int n);
    virtual QVector<WidgetData*>* getRow(int row);
    virtual WidgetData* at(int row, int n);
    void beginEdit();
    void markRow(int begin, int end);
    void markColumn(int row, int begin, int end);
    void endEdit();
    int getColumnChangeBegin() const;
    int getColumnChangeEnd() const;
    int getDirtyRow() const;
    int getRowChangeBegin() const;
    int getRowChangeEnd() const;
    EditType getEditType() const;
};

#endif //LIFERHYTHM_NESTEDLISTDATA_H