//
// Created by stupid_coder_jyy on 2024/4/27.
//

#include "UsageTypeLabel.h"
#include "Usage.h"
#include "Styles.h"
#include <QPainter>
#include <QPainterPath>

USING_NAMESPACE(lr::log)

UsageTypeLabel::UsageTypeLabel(QWidget *parent): Widget(parent), bgColor(), name(), shouldDraw() {
}

void UsageTypeLabel::syncDataToWidget() {
    shouldDraw = wData;
    if (wData) {
        auto* u = wData->cast<UsageType>();
        name = u->getName();
        QFontMetrics fm(Styles::FONT_MAIN);
        setFixedSize(fm.horizontalAdvance(name) + 6, fm.height() + 4);
        bgColor = u->getColor();
    }
    update();
}

void UsageTypeLabel::paintEvent(QPaintEvent *event) {
    if (!shouldDraw) {
        return;
    }
    QPainter p(this);
    QPainterPath path;
    path.addRoundedRect(rect(), 3, 3);
    p.fillPath(path, bgColor);
    p.setFont(Styles::FONT_MAIN);
    p.setPen(Styles::GRAY_TEXT_0->color);
    p.drawText(rect().adjusted(3, 0, -3, 0), Qt::AlignCenter, name);
}

void UsageTypeLabel::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, &UsageTypeLabel::syncDataToWidget);
}
