//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef LIFERHYTHM_SLOTSDRAWER_H
#define LIFERHYTHM_SLOTSDRAWER_H

#include "Widget.h"

class DrawerLayer;

class SlotsDrawer : public Widget {
protected:
    const int slotWidth;
    const int slotHeight;
    const int columns;
    const int rows;
private:
    bool init;
    QVector<DrawerLayer*> layers;
public:
    explicit SlotsDrawer(int slotsWidth, int slotsHeight, int columns, int rows, QWidget* parent = nullptr);
    ~SlotsDrawer() override;
    void paintEvent(QPaintEvent *event) override;
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    virtual void initLayers() = 0;
    virtual void onSlotClicked(QMouseEvent* evt, int row, int column);
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

inline void SlotsDrawer::appendLayer(DrawerLayer *layer) {
    layer->parent = this;
    layers << layer;
}

inline void SlotsDrawer::insertLayer(int i, DrawerLayer *layer) {
    layer->parent = this;
    layers.insert(i, layer);
}

#endif //LIFERHYTHM_SLOTSDRAWER_H
