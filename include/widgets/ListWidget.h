//
// Created by stupid_coder_jyy on 2024/2/25.
//

#ifndef LIFERHYTHM_LISTWIDGET_H
#define LIFERHYTHM_LISTWIDGET_H

#include <QScrollArea>
#include "ScrollBar.h"
#include "Widget.h"
#include "WidgetData.h"
#include "QVBoxLayout"

class ListItem : public Widget {
    Q_OBJECT
    friend class ListWidget;
protected:
    int dataIdx;
    WidgetData* data;
private:
    QMetaObject::Connection dc;
public:
    explicit ListItem(QWidget* parent = nullptr);
public slots:
    virtual void syncDataToWidget();    //将WidgetData中的数据同步到控件中，ListWidget内部调用，也可以手动调用
    virtual void syncWidgetToData();    //将控件的数据同步到WidgetData中，需要手动调用
    virtual void clearWidget();         //将控件恢复到无数据的状态，ListWidget内部调用
private:
    void setData(WidgetData* d);
};

class ListWidget : public QScrollArea, public StandardWidget{
    Q_OBJECT
    SCROLL_CLAZZ_DEF
private:
    QVector<ListItem*> items;
    QMetaObject::Connection mc;
    IListModel* model;
    QVBoxLayout* layout;
    QWidget* container;
    int rowHeight;
    int areaRowCount;
    int pos;
    int globalPos;
    int maxGlobalPos;
    int posMid, posBottom;
    int idxA, idxB;
public:
    explicit ListWidget(QWidget* parent = nullptr);
    void setRowHeight(int s);
    void setMinAreaRowCount(int count);
    void setModel(IListModel* model);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
protected:
    virtual ListItem* createRowItem();
    void wheelEvent(QWheelEvent *event) override;
private:
    void updateListBase();
    void appendItem();
    void fillA(int begin, bool force = false);
    void fillB(int begin, bool force = false);
    void setItemData(ListItem* item, int idx);
    void updateMaxGlobalPos();
private slots:
    void setGlobalPos(int globalPos, bool force = false);
    void onDataChanged(int begin, int end);
};


#endif //LIFERHYTHM_LISTWIDGET_H
