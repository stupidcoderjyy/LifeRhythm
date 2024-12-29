//
// Created by JYY on 24-12-14.
//

#ifndef SLOTSPAINTERWEEKDAYS_H
#define SLOTSPAINTERWEEKDAYS_H
#include <Namespaces.h>
#include "SlotsPainter.h"

BEGIN_NP(lr::calendar)

class LayerWeekdays : public SlotsPainterLayer {
private:
    int begin;
public:
    LayerWeekdays();
    void set(int begin);
protected:
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int column, int row) override;
};

END_NP

#endif //SLOTSPAINTERWEEKDAYS_H
