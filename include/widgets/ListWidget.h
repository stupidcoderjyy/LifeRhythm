//
// Created by stupid_coder_jyy on 2024/2/25.
//

#ifndef LIFERHYTHM_LISTWIDGET_H
#define LIFERHYTHM_LISTWIDGET_H

#include "Widget.h"
#include "ListData.h"
#include "ScrollArea.h"
#include <QTimer>

class CORE_API ListItem : public Widget {
    Q_OBJECT
    friend class ListWidget;
    friend class TiledListWidget;
    friend class TreeWidget;
    friend class SlotsWidget;
protected:
    ListData* listData;
    bool selected;
    int dataIdx;
private:
    QPoint dragStart;
public:
    explicit ListItem(QWidget* parent = nullptr);
    void syncDataToWidget() override;
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
signals:
    void sigDragStart(ListItem* item);
    void sigDragEnd(ListItem* item);
    void sigDragEnter(ListItem* item);
    void sigDragLeave(ListItem* item);
    void sigDragMove(ListItem* item, QDragMoveEvent *event);
    void sigDropped(ListItem* item);
private:
    void setList(ListData* data) {
        this->listData = data;
    }
};

class CORE_API ListWidget : public ScrollArea {
    Q_OBJECT
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
    int idxA, idxB;
public:
    explicit ListWidget(QWidget* parent = nullptr);
    void setRowHeight(int s);
    void setMinAreaRowCount(int count);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void setData(WidgetData* d) override;
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
    void setGlobalPos(int globalPos, bool forceUpdate = false);
    void onDataChanged(int begin, int end);
    void scroll(int dy);
    void connectModelView() override;
private:
    void updateListBase();
    void fillA(int begin, bool forceUpdate = false);
    void fillB(int begin, bool forceUpdate = false);
    void setItemData(ListItem* item, int idx) const;
    void updateMaxGlobalPos();
    void performDragScroll(const ListItem* src, const QDragMoveEvent *event);
};

#endif //LIFERHYTHM_LISTWIDGET_H
