//
// Created by stupid_coder_jyy on 2024/4/15.
//

#include "TimeScale.h"
#include "Styles.h"

USING_NAMESPACE(lr::log)

TimeScale::TimeScale(QWidget *parent): QWidget(parent), vpp(-1) {
    setFixedWidth(50);
}

void TimeScale::setVpp(double v3) {
    vpp = v3;
    update();
}

void TimeScale::paintEvent(QPaintEvent *event) {
    if (vpp < 0) {
        return;
    }
    QPainter p(this);
    QPen pen;
    pen.setColor(Styles::GRAY_3->color);
    p.setPen(pen);
    p.setFont(Styles::FONT_SMALL);
    for (int hour = 1 ; hour < 24 ; hour ++) {
        QString str = hour == 12 ? "正午" : QString::number(hour).append(":00");
        QRect r(0, qRound(hour * 60 * vpp) - 20, width(), 40);
        p.drawText(r, Qt::AlignVCenter | Qt::AlignRight, str);
    }
}