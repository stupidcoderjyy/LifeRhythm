//
// Created by stupid_coder_jyy on 2024/4/15.
//

#ifndef LIFERHYTHM_WEEKVIEWTITLE_H
#define LIFERHYTHM_WEEKVIEWTITLE_H

#include "Namespaces.h"
#include "AbstractSlotsDrawer.h"

BEGIN_NAMESPACE(lr::log)

class WeekViewTitle : public AbstractSlotsDrawer {
public:
    explicit WeekViewTitle(QWidget* parent = nullptr);
protected:
    void beforeDrawing(QPainter &p, QRect &area) override;
    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
};

END_NAMESPACE

#endif //LIFERHYTHM_WEEKVIEWTITLE_H
