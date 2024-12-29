//
// Created by stupid_coder_jyy on 2024/2/25.
//

#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include "Widget.h"
#include "ListData.h"
#include "ScrollArea.h"
#include <QTimer>

BEGIN_LR

class WidgetFactory;

class ListItem : public Widget {
    Q_OBJECT
    friend class ListWidget;
    friend class TiledListWidget;
    friend class TreeWidget;
    friend class SlotsWidget;
protected:
    ListData* listData;
    bool selected;
    int dataIdx;
public:
    explicit ListItem(QWidget* parent = nullptr, bool iic = true);
    void syncDataToWidget() override;
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    void setList(ListData* data) {
        this->listData = data;
    }
};

class ListWidget : public ScrollArea {
    Q_OBJECT
protected:
    QVector<ListItem*> items;
    QWidget* container;
    int dragScrollStep;
    int rowHeight;
    int areaRowCount;
    int pos;
    int globalPos;
    int maxGlobalPos;
    int posMid, posBottom;
    int idxA, idxB;
    WidgetFactory* factoryItem;
public:
    explicit ListWidget(QWidget* parent = nullptr, bool iic = true);
    void setRowHeight(int s);
    void setMinAreaRowCount(int count);
    void onPostParsing(Handlers &handlers, NBT *nbt) override;
    void setData(WidgetData* d) override;
    ~ListWidget() override;
protected:
    virtual ListItem* newItem();
    virtual void prepareItem(ListItem* item);
    virtual void syncItem(ListItem *item, int idx);
    void setGlobalPos(int globalPos, bool forceUpdate = false);
    void syncItems(int begin, int end);
    void scroll(int dy);
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    ScrollBar *createVerticalScrollBar() override;
    void connectModelView() override;
private:
    void updateBase();
    void fillA(int begin, bool forceUpdate = false);
    void fillB(int begin, bool forceUpdate = false);
    void updateMaxGlobalPos();
};

END_LR

#endif //LISTWIDGET_H
