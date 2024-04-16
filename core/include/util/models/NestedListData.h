//
// Created by stupid_coder_jyy on 2024/4/2.
//

#ifndef LIFERHYTHM_NESTEDLISTDATA_H
#define LIFERHYTHM_NESTEDLISTDATA_H

#include "ListData.h"

class NestedListNode : public WidgetData {
    Q_OBJECT
    friend class NestedListData;
protected:
    int row;    //由NestedListData设置，如果需要更改调用changeRow
public:
    NestedListNode();
    inline int getRow() const;
    inline void changeRow(int row);
protected:
signals:
    void sigRowChanged(int old, int cur);
};

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
    QVector<QVector<NestedListNode*>*> data;
public:
    NestedListData();
    ~NestedListData() override;
    virtual void removeRow(int idx);
    virtual void insertRow(int idx, int count);
    inline void insertRow(int idx);
    virtual void makeRow(int count);
    inline void makeRow();
    virtual void append(int row, NestedListNode* d);
    virtual void insert(int row, int n, NestedListNode* d);
    virtual NestedListNode* remove(int row, int n);
    inline QVector<NestedListNode*>* getRow(int row);
    inline NestedListNode* at(int row, int n);
    void beginEdit();
    void markRow(int begin, int end);
    void markColumn(int row, int begin, int end);
    void markAll();
    void endEdit();
    inline int getColumnChangeBegin() const;
    inline int getColumnChangeEnd() const;
    inline int getDirtyRow() const;
    inline int getRowChangeBegin() const;
    inline int getRowChangeEnd() const;
    inline EditType getEditType() const;
private:
    void initNode(NestedListNode* node, int row);
};

inline int NestedListNode::getRow() const {
    return row;
}

inline void NestedListNode::changeRow(int r) {
    if (row != r) {
        emit sigRowChanged(row, r);
        row = r;
    }
}

inline void NestedListData::insertRow(int idx) {
    insertRow(idx, 1);
}

inline void NestedListData::makeRow() {
    makeRow(1);
}

inline QVector<NestedListNode*> *NestedListData::getRow(int row) {
    return data[row];
}

inline NestedListNode*NestedListData::at(int row, int n) {
    if (row >= data.length()) {
        return nullptr;
    }
    auto* c = data[row];
    if (n >= c->length()) {
        return nullptr;
    }
    return c->at(n);
}

inline int NestedListData::getColumnChangeBegin() const {
    return columnChangeBegin;
}

inline int NestedListData::getColumnChangeEnd() const {
    return columnChangeEnd;
}

inline int NestedListData::getDirtyRow() const {
    return dirtyRow;
}

inline int NestedListData::getRowChangeBegin() const {
    return rowChangeBegin;
}

inline int NestedListData::getRowChangeEnd() const {
    return rowChangeEnd;
}

inline NestedListData::EditType NestedListData::getEditType() const {
    return editType;
}

#endif //LIFERHYTHM_NESTEDLISTDATA_H