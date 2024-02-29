//
// Created by stupid_coder_jyy on 2024/2/28.
//

#include "ArrowButton.h"
#include "Styles.h"
#include <QResizeEvent>
#include <QPainter>

ArrowButton::ArrowButton(Type type, QWidget *parent): QWidget(parent), type(type),
        ratioWidth(0.75), ratioHeight(0.4), activatedType(type), pressed(), path(),
        points(), scale(1.0) {
}

void ArrowButton::setActivatedType(ArrowButton::Type t) {
    activatedType = t;
}

void ArrowButton::setRatioWidth(double rw) {
    ratioWidth = rw;
}

void ArrowButton::setRatioHeight(double rh) {
    ratioHeight = rh;
}

void ArrowButton::setRatio(double rw, double rh) {
    ratioWidth = rw;
    ratioHeight = rh;
}

void ArrowButton::setScale(double s) {
    scale = s;
}

void ArrowButton::setPressed(bool p) {
    if (pressed != p) {
        updatePoints(p ? activatedType : type);
        pressed = p;
    }
}

void ArrowButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(Styles::GRAY_TEXT_0);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(points[0], points[1], points[2], points[3]);
    painter.drawLine(points[2], points[3], points[4], points[5]);
    painter.setRenderHint(QPainter::Antialiasing, false);
}

void ArrowButton::mousePressEvent(QMouseEvent *event) {
    pressed = !pressed;
    if (activatedType != type) {
        updatePoints(pressed ? activatedType : type);
    }
    emit sigActivated(pressed);
}

void ArrowButton::showEvent(QShowEvent *event) {
    updatePoints(type, false);
}

void ArrowButton::updatePoints(Type t, bool u) {
    auto s = size();
    int w0 = qRound(s.width() * ratioWidth * scale);
    int h0 = qRound(s.height() * ratioHeight * scale);
    if (t & 2) {
        int w = h0;
        int h = w0;
        points[1] = (s.height() - h) >> 1;
        points[3] = s.height() >> 1;
        points[5] = s.height() - points[1];
        if (t == Right) {
            points[0] = (s.width() - w) >> 1;
            points[2] = s.width() - points[0];
        } else {
            points[2] = (s.width() - w) >> 1;
            points[0] = s.width() - points[2];
        }
        points[4] = points[0];
    } else {
        int h = h0;
        int w = w0;
        points[0] = (s.width() - w) >> 1;
        points[2] = s.width() >> 1;
        points[4] = s.width() - points[0];
        if (t == Up) {
            points[3] = (s.height() - h) >> 1;
            points[1] = s.height() - points[3];
        } else {
            points[1] = (s.height() - h) >> 1;
            points[3] = s.height() - points[1];
        }
        points[5] = points[1];
    }
    if (u) {
        update();
    }
}
