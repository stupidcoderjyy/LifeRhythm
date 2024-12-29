//
// Created by JYY on 24-11-4.
//

#ifndef SLOTSWIDGET_H
#define SLOTSWIDGET_H

#include <ListWidget.h>
#include <Widget.h>
#include <Error.h>

BEGIN_LR

class SlotsWidget : public Widget {
public:
    enum SlotSizePolicy {
        Fixed,
        Auto
    };
protected:
    QVector<ListItem*> items;
    int slotWidth, slotHeight;
    int columns, rows;
    SlotSizePolicy vSlotSizePolicy, hSlotSizePolicy;
    WidgetFactory *factoryItem;
    QMargins slotMargins;
    QMargins regionMargins;
    QSize region;
public:
    explicit SlotsWidget(QWidget *parent = nullptr, bool iic = true);
    void setData(WidgetData *d) override;
    void setItemFactory(WidgetFactory *factory);
    ~SlotsWidget() override;
    inline void setSlotSize(int width, int height);
    inline void setSlotCount(int columns, int rows);
    inline void setSlotSizePolicy(SlotSizePolicy horizontal, SlotSizePolicy vertical);
    inline void setSlotMargins(int left, int top, int right, int bottom);
    inline void setRegionMargins(int left, int top, int right, int bottom);
    inline void syncItems() const;
protected:
    virtual void prepareItem(ListItem* w);
    virtual void syncItem(int idx) const;
    virtual ListItem* newItem();
    virtual void updateBase();
    void syncItems(int begin, int end) const;
    void resizeEvent(QResizeEvent* event) override;
    void connectModelView() override;
    void onPostParsing(Handlers &handlers, NBT *nbt) override;
};

inline void SlotsWidget::setSlotSize(int width, int height) {
    if (width <= 0 || height <= 0) {
        throw Error("SlotsWidget::setSlotSize invalid size");
    }
    slotWidth = width;
    slotHeight = height;
    updateBase();
}

inline void SlotsWidget::setSlotCount(int c, int r) {
    columns = c;
    rows = r;
    updateBase();
}

inline void SlotsWidget::setSlotSizePolicy(SlotSizePolicy horizontal, SlotSizePolicy vertical) {
    vSlotSizePolicy = vertical;
    hSlotSizePolicy = horizontal;
    updateBase();
}

inline void SlotsWidget::setSlotMargins(int left, int top, int right, int bottom) {
    slotMargins = QMargins(left, top, right, bottom);
    updateBase();
}

inline void SlotsWidget::setRegionMargins(int left, int top, int right, int bottom) {
    regionMargins = QMargins(left, top, right, bottom);
    updateBase();
}

inline void SlotsWidget::syncItems() const {
    syncItems(0, INT32_MAX);
}

END_LR

#endif //SLOTSWIDGET_H
