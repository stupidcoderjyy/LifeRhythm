//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef LIFERHYTHM_SLOTSDRAWER_H
#define LIFERHYTHM_SLOTSDRAWER_H

#include "Widget.h"

class DrawerLayer;

class SlotsDrawer : public Widget {
protected:
    int slotWidth;
    int slotHeight;
    int columns;
    int rows;
private:
    bool shouldResize;
    bool init;
    QVector<DrawerLayer*> layers;
public:
    explicit SlotsDrawer(QWidget* parent = nullptr);
    ~SlotsDrawer() override;
    void paintEvent(QPaintEvent *event) override;
    inline void setSlotSize(int width, int height);
    inline void setSlotCount(int columns, int rows);
protected:
    void resizeEvent(QResizeEvent *event) override;
    virtual void initLayers() = 0;
    inline void appendLayer(DrawerLayer* layer);
    inline void insertLayer(int i, DrawerLayer* layer);
};

class DrawerLayer {
    friend class SlotsDrawer;
protected:
    SlotsDrawer* parent;
public:
    DrawerLayer();
protected:
    virtual bool shouldDraw();
    virtual void beforeDrawing(QPainter& p, QRect& area);
    virtual void drawSlot(QPainter& p, QRect& area, int row, int column);
    virtual void afterDrawing(QPainter& p);
};

inline void SlotsDrawer::setSlotSize(int w, int h) {
    if (slotWidth != w || slotHeight != h) {
        slotWidth = w;
        slotHeight = h;
        shouldResize = true;
    }
}

inline void SlotsDrawer::setSlotCount(int c, int r) {
    if (columns != c || rows != r) {
        columns = c;
        rows = r;
        shouldResize = true;
    }
}

inline void SlotsDrawer::appendLayer(DrawerLayer *layer) {
    layer->parent = this;
    layers << layer;
}

inline void SlotsDrawer::insertLayer(int i, DrawerLayer *layer) {
    layer->parent = this;
    layers.insert(i, layer);
}

#endif //LIFERHYTHM_SLOTSDRAWER_H
