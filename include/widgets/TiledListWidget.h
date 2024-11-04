//
// Created by JYY on 24-11-2.
//

#ifndef TILEDLISTWIDGET_H
#define TILEDLISTWIDGET_H

#include <QTimer>
#include "ListWidget.h"

class TiledListWidget : public ScrollArea {
public:
    enum ColumnPolicy {
        Fixed,
        Auto
    };
protected:
    QVector<ListItem*> items;
    QWidget* container;
    QTimer scrollTimer;
    int dragScrollStep;
    int rowHeight;
    int areaRowCount;
    int pos;
    int globalPos;
    int maxGlobalPos;
    int posMid, posBottom;
    int idxA, idxB;     //行号
    int columnCount;    //固定或自动设置
    int columnWidth;    //固定宽度
    ColumnPolicy columnPolicy;
public:
    explicit TiledListWidget(QWidget* parent = nullptr);
    void setRowHeight(int s);
    void setMinAreaRowCount(int count);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void setData(WidgetData* d) override;
    void setColumnPolicy(ColumnPolicy policy);
    void setColumnWidth(int width);
    void setColumnCount(int count);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    ScrollBar* createVerticalScrollBar() override;
    virtual ListItem* createRowItem();
    virtual void prepareNewItem(ListItem* item);
    virtual void onItemDragStart(ListItem* item);
    virtual void onItemDragEnter(ListItem* item);
    virtual void onItemDragLeave(ListItem* item);
    virtual void onItemDragMove(ListItem* item, QDragMoveEvent *event);
    virtual void onItemDropped(ListItem* src, ListItem* dest);
    void setGlobalPos(int globalPos, bool forceUpdate);
    void onDataChanged(int begin, int end);
    void scroll(int dy);
    void connectModelView() override;
    void setItemData(ListItem* item, int idx) const;
private:
    void updateListBase();
    void fillA(int beginRow, bool forceUpdate = false);
    void fillB(int beginRow, bool forceUpdate = false);
    void updateMaxGlobalPos();
    void performDragScroll(const ListItem* src, const QDragMoveEvent *event);
    int getMaxRowCount(int i) const {
        return i % columnCount == 0 ? i / columnCount : i / columnCount + 1;
    }
};



#endif //TILEDLISTWIDGET_H
