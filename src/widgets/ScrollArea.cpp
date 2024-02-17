//
// Created by stupid_coder_jyy on 2024/2/12.
//

#include "widgets/ScrollArea.h"
#include "NBT.h"
#include "Styles.h"
#include <QDebug>
#include <QWheelEvent>

ScrollArea::ScrollArea(QWidget *parent):QScrollArea(parent),StandardWidget() {
    setFrameShape(QFrame::NoFrame);
    setWidgetResizable(true);
    viewport()->setObjectName("vp");
    viewport()->setStyleSheet(qss_t("vp", bg(Styles::CLEAR)));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vBar = new ScrollBar(this,Qt::Vertical,verticalScrollBar());
    vBarAnimation.setTargetObject(verticalScrollBar());
    vBarAnimation.setPropertyName("value");
    vBarAnimation.setEasingCurve(QEasingCurve::Linear);
    hBar = new ScrollBar(this,Qt::Horizontal,horizontalScrollBar());
    hBarAnimation.setTargetObject(horizontalScrollBar());
    hBarAnimation.setPropertyName("value");
    hBarAnimation.setEasingCurve(QEasingCurve::Linear);
    connect(&hBarAnimation, &QPropertyAnimation::finished, this, [this](){
        speed = 0;
    });
    connect(&vBarAnimation, &QPropertyAnimation::finished, this, [this](){
        speed = 0;
    });
}

void ScrollArea::resizeEvent(QResizeEvent *event) {
    hBar->setGeometry(0, height() - 8, width(), 7);
    vBar->setGeometry(width() - 8, 0, 7, height());
    QScrollArea::resizeEvent(event);
}

void ScrollArea::wheelEvent(QWheelEvent *event) {
    QPropertyAnimation* animation;
    QScrollBar* bar;
    if (event->modifiers() == Qt::ShiftModifier) {
        animation = &hBarAnimation;
        bar = hBar;
    } else {
        animation = &vBarAnimation;
        bar = vBar;
    }
    if (animation->state() != QAbstractAnimation::Stopped) {
        animation->stop();
        animation->setDuration(qMax(40, 100 - speed++ * 10));
    } else {
        animation->setDuration(200);
    }
    animation->setStartValue(bar->value());
    animation->setEndValue(bar->value() - event->angleDelta().y());

    animation->start();
}