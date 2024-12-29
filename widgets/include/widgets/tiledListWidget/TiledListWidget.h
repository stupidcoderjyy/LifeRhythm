//
// Created by JYY on 24-11-2.
//

#ifndef TILEDLISTWIDGET_H
#define TILEDLISTWIDGET_H

#include "ListWidget.h"

BEGIN_LR

class TiledListWidget : public ScrollArea {
public:
    enum ColumnPolicy {
        Fixed,
        Auto
    };
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
    int idxA, idxB;     //行号
    int columnCount;    //固定或自动设置
    int columnWidth;    //固定宽度
    ColumnPolicy columnPolicy;
public:
    explicit TiledListWidget(QWidget* parent = nullptr, bool initInConstructor = true);
    void setSlotSize(int width, int height);
    void setColumnCount(int columns);
    void setColumnPolicy(ColumnPolicy policy);
    void setData(WidgetData *d) override;
protected:
    void setGlobalPos(int globalPos, bool forceUpdate);
    void scroll(int dy);
    void syncItems(int begin, int end);
    virtual void syncItem(ListItem* item, int dataIdx) const;
    virtual ListItem *newItem();
    virtual void prepareItem(ListItem *item);
    void connectModelView() override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    ScrollBar *createVerticalScrollBar() override;
private:
    void updateBase();
    void fillA(int beginRow, bool forceUpdate = false);
    void fillB(int beginRow, bool forceUpdate = false);
    void updateMaxGlobalPos();
    int getMaxRowCount(int i) const {
        return i % columnCount == 0 ? i / columnCount : i / columnCount + 1;
    }
};

END_LR

#endif //TILEDLISTWIDGET_H
