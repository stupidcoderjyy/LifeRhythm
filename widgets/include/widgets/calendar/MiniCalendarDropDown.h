//
// Created by JYY on 24-12-13.
//

#ifndef CALENDARPRIVATE_H
#define CALENDARPRIVATE_H

#include <Namespaces.h>
#include <ListData.h>
#include <DropDown.h>
#include <MiniCalendar.h>
#include <Common.h>

BEGIN_NP(lr::calendar)

class MiniCalendarDropDown : public MiniCalendar {
private:
    LayerDay* layerDay;
    LayerMonth* layerMonth;
    QDate dateStart;
    ViewType viewType;
    bool isViewTypeMonth;
public:
    explicit MiniCalendarDropDown(QWidget* parent = nullptr, bool iic = true);
    void syncDataToWidget() override;
    void syncWidgetToData() override;
protected:
    void initWidget() override;
    void syncWidget() override;
};

class LayerDay : public SlotsPainterLayer {
private:
    int len;
    int count;
    int hb, he;
    int sb, se;
public:
    LayerDay();
    void set(int len, int sb);
private:
    void mouseEntered(int column, int row) override;
    void mouseLeaved() override;
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int column, int row) override;
};

class LayerMonth : public SlotsPainterLayer {
private:
    int hovered;
    int selected;
    int count;
    bool isViewTypeMonth;
public:
    LayerMonth();
    void set(bool isViewTypeMonth, int s);
    void mouseEntered(int column, int row) override;
    void mouseLeaved() override;
    bool shouldDraw() override;
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int column, int row) override;
};

END_NP

#endif //CALENDARPRIVATE_H
