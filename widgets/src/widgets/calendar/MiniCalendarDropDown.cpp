//
// Created by JYY on 24-12-13.
//

#include "MiniCalendarDropDown.h"
#include "Calendar.h"

USING_NP(lr::calendar)

MiniCalendarDropDown::MiniCalendarDropDown(QWidget *parent, bool iic): MiniCalendar(parent, iic),layerDay(), layerMonth(), viewType(), isViewTypeMonth() {
}

void MiniCalendarDropDown::syncDataToWidget() {
    if (auto md = wData->cast<CalendarData>()) {
        dateStart = md->getDateStart();
        viewType = md->getViewType();
        isViewTypeMonth = viewType == Month;
        if (viewType == Month) {
            setMaxViewLevel(ViewLevel::Month);
        } else {
            setMaxViewLevel(ViewLevel::Day);
        }
    }
}

void MiniCalendarDropDown::syncWidgetToData() {
    if (auto md = wData->cast<CalendarData>()) {
        md->set(viewType, dateStart);
    }
}

void MiniCalendarDropDown::initWidget() {
    MiniCalendar::initWidget();
    auto p = painters[ViewLevel::Day];
    layerDay = new LayerDay;
    p->addLayer(layerDay);
    p->setMouseTracking(true);
    connect(this, &MiniCalendar::sigPress, [this](const QDate &d) {
        if (viewLevel == ViewLevel::Day) {
            dateStart = d;
            syncWidget();
        }
    });
    p = painters[ViewLevel::Month];
    layerMonth = new LayerMonth;
    p->addLayer(layerMonth);
    p->setMouseTracking(true);
    connect(this, &MiniCalendar::sigPress, [this](const QDate &d) {
        if (viewLevel == ViewLevel::Month && isViewTypeMonth) {
            dateStart = d;
            syncWidget();
        }
    });
}

void MiniCalendarDropDown::syncWidget() {
    MiniCalendar::syncWidget();
    if (!prepared) {
        return;
    }
    layerDay->set(viewType, dateTopLeft.daysTo(dateStart));
    if (isViewTypeMonth) {
        int years = dateStart.year() - dateTopLeft.year();
        int months = dateStart.month() - dateTopLeft.month();
        layerMonth->set(true, years * 12 + months);
    } else {
        layerMonth->set(false, -1);
    }
}

LayerDay::LayerDay(): len(0), count(), hb(-1), he(-1), sb(-1), se(-1) {
}

void LayerDay::set(int i0, int i1) {
    len = i0;
    sb = i1;
    se = sb + len - 1;
    parent->update();
}

void LayerDay::mouseEntered(int column, int row) {
    if (len > 0) {
        hb = row * 7 + column;
        he = hb + len - 1;
    } else {
        hb = -1;
        he = -1;
    }
    parent->update();
}

void LayerDay::mouseLeaved() {
    hb = -1;
    he = -1;
    parent->update();
}

void LayerDay::beforeDrawing(QPainter &p) {
    count = 0;
}

#define BD 2

void LayerDay::drawSlot(QPainter &p, QRect &area, int column, int row) {
    bool hovered = count >= hb && count <= he;
    bool selected = count >= sb && count <= se;
    if (selected) {
        p.fillRect(area, Styles::BLUE_0->color);
        if (hovered) {
            if (count == sb) {
                if (len == 1) {
                    p.fillRect(area.adjusted(BD, BD, -BD, -BD), Styles::GRAY_0->color);
                } else {
                    p.fillRect(area.adjusted(BD, BD, 0, -BD), Styles::GRAY_0->color);
                }
            } else if (count == se) {
                p.fillRect(area.adjusted(0, BD, -BD, -BD), Styles::GRAY_0->color);
            } else {
                p.fillRect(area.adjusted(0, BD, 0, -BD), Styles::GRAY_0->color);
            }
        }
    } else if (hovered) {
        p.fillRect(area, Styles::GRAY_4->color);
        if (count == hb) {
            if (len == 1) {
                p.fillRect(area.adjusted(BD, BD, -BD, -BD), Styles::GRAY_0->color);
            } else {
                p.fillRect(area.adjusted(BD, BD, 0, -BD), Styles::GRAY_0->color);
            }
        } else if (count == he) {
            p.fillRect(area.adjusted(0, BD, -BD, -BD), Styles::GRAY_0->color);
        } else {
            p.fillRect(area.adjusted(0, BD, 0, -BD), Styles::GRAY_0->color);
        }
    }
    count++;
}

LayerMonth::LayerMonth(): hovered(-1), selected(-1), count(), isViewTypeMonth() {
}

void LayerMonth::set(bool i, int s) {
    isViewTypeMonth = i;
    selected = s;
    parent->update();
}

void LayerMonth::mouseEntered(int column, int row) {
    if (isViewTypeMonth) {
        hovered = (row << 2) + column;
        parent->update();
    }
}

void LayerMonth::mouseLeaved() {
    if (isViewTypeMonth) {
        hovered = -1;
        parent->update();
    }
}

bool LayerMonth::shouldDraw() {
    return isViewTypeMonth;
}

void LayerMonth::beforeDrawing(QPainter &p) {
    count = 0;
}

void LayerMonth::drawSlot(QPainter &p, QRect &area, int column, int row) {
    if (!isViewTypeMonth) {
        return;
    }
    if (count == selected) {
        p.fillRect(area, Styles::BLUE_0->color);
        if (count == hovered) {
            p.fillRect(area.adjusted(BD, BD, -BD, -BD), Styles::GRAY_0->color);
        }
    } else if (count == hovered) {
        p.fillRect(area, Styles::GRAY_4->color);
        p.fillRect(area.adjusted(BD, BD, -BD, -BD), Styles::GRAY_0->color);
    }
    count++;
}
