//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef LIFERHYTHM_ABSTRACTSLOTSDRAWER_H
#define LIFERHYTHM_ABSTRACTSLOTSDRAWER_H

#include "Widget.h"

class AbstractSlotsDrawer : public Widget {
protected:
    int slotWidth;
    int slotHeight;
    int columns;
    int rows;
    bool shouldResize;
public:
    explicit AbstractSlotsDrawer(QWidget* parent = nullptr);
    inline void setSlotSize(int width, int height);
    inline void setSlotCount(int columns, int rows);
protected:
    virtual void beforeDrawing(QPainter& p, QRect& area);
    virtual void drawSlot(QPainter& p, QRect& area, int row, int column) = 0;
    virtual void afterDrawing(QPainter& p);
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif //LIFERHYTHM_ABSTRACTSLOTSDRAWER_H
