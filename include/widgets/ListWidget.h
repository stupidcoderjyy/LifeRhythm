//
// Created by stupid_coder_jyy on 2024/2/25.
//

#ifndef LIFERHYTHM_LISTWIDGET_H
#define LIFERHYTHM_LISTWIDGET_H

#include "ScrollBar.h"
#include "Widget.h"
#include "models/ListModel.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QTimer>
#include <QDrag>

class ListItem : public Widget {
    Q_OBJECT
    friend class ListWidget;
protected:
    int dataIdx;
    WidgetData* data;
private:
    QPoint dragStart;
    QMetaObject::Connection dc;
public:
    explicit ListItem(QWidget* parent = nullptr);
public:
    virtual void syncDataToWidget();    //将WidgetData中的数据同步到控件中，ListWidget内部调用，也可以手动调用
    virtual void syncWidgetToData();    //将控件的数据同步到WidgetData中，需要手动调用
    virtual void clearWidget();         //将控件恢复到无数据的状态，ListWidget内部调用
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    void setData(WidgetData* d);
signals:
    void sigDragStart(ListItem* item);
    void sigDragEnd(ListItem* item);
    void sigDragEnter(ListItem* item);
    void sigDragLeave(ListItem* item);
    void sigDragMove(ListItem* item, QDragMoveEvent *event);
    void sigDropped(ListItem* item);
};

class ListWidget : public QScrollArea, public StandardWidget{
    Q_OBJECT
    SCROLL_CLAZZ_DEF
protected:
    QVector<ListItem*> items;
    QMetaObject::Connection mc;
    IListModel* model;
    QVBoxLayout* layout;
    QWidget* container;
    QTimer scrollTimer;
    int dragScrollStep;
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
    virtual void prepareNewItem(ListItem* item);
    void wheelEvent(QWheelEvent *event) override;
    virtual void onItemDragStart(ListItem* item);
    virtual void onItemDragEnter(ListItem* item);
    virtual void onItemDragLeave(ListItem* item);
    virtual void onItemDragMove(ListItem* item, QDragMoveEvent *event);
    virtual void onItemDropped(ListItem* src, ListItem* dest);
    void setGlobalPos(int globalPos, bool forceUpdate = false);
    void onDataChanged(int begin, int end);
private:
    void updateListBase();
    void fillA(int begin, bool forceUpdate = false);
    void fillB(int begin, bool forceUpdate = false);
    void setItemData(ListItem* item, int idx);
    void updateMaxGlobalPos();
    void performDragScroll(ListItem* src, QDragMoveEvent *event);
};

#endif //LIFERHYTHM_LISTWIDGET_H
