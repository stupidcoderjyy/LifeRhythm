//
// Created by JYY on 24-12-13.
//

#include "LoadingIcon.h"

#include <QTimer>

USING_LR

LoadingIcon::LoadingIcon(QWidget *parent, bool iic): Widget(parent, iic), angle() {
    connect(&timer, &QTimer::timeout, this, &LoadingIcon::updateRotation);
    setFixedSize(20, 20);
    close();
}

void LoadingIcon::startLoading() {
    timer.start(10);
    show();
}

void LoadingIcon::stopLoading() {
    timer.stop();
    close();
}

void LoadingIcon::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int radius = 6;
    int thickness = 3;
    painter.translate(width() / 2, height() / 2);

    QPen pen;
    pen.setColor(Styles::GRAY_2->color);
    pen.setWidth(thickness);
    painter.setPen(pen);
    painter.drawArc(-radius, -radius, 2 * radius, 2 * radius, 0 * 16, 360 * 16);

    pen.setColor(Styles::GRAY_TEXT_0->color);
    painter.setPen(pen);

    painter.drawArc(-radius, -radius, 2 * radius, 2 * radius, -angle * 16, -60 * 16);
    painter.setRenderHint(QPainter::Antialiasing, false);
}

void LoadingIcon::updateRotation() {
    angle += 3;
    if (angle >= 360) {
        angle = 0;
    }
    update();
}
