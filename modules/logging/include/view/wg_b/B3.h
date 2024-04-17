//
// Created by stupid_coder_jyy on 2024/4/15.
//

#ifndef LIFERHYTHM_B3_H
#define LIFERHYTHM_B3_H

#include "Namespaces.h"
#include "SlotsDrawer.h"
#include <QDate>

BEGIN_NAMESPACE(lr::log)

class B3 : public SlotsDrawer {
private:
    int months[7];
    int days[7];
public:
    explicit B3(QWidget* parent = nullptr);
    void syncDataToWidget() override;
    void initLayers() override;
};

class WeekViewTitleLayer : public DrawerLayer {
private:
    int* months;
    int* days;
public:
    WeekViewTitleLayer(int* months, int* days);
protected:
    void beforeDrawing(QPainter &p, QRect &area) override;
    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
};

END_NAMESPACE

#endif //LIFERHYTHM_B3_H
