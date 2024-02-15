//
// Created by stupid_coder_jyy on 2024/2/12.
//

#ifndef LIFERHYTHM_SCROLLBAR_H
#define LIFERHYTHM_SCROLLBAR_H

#include <QScrollBar>
#include <QPropertyAnimation>
#include <QWheelEvent>

class ScrollBar : public QScrollBar{
    Q_OBJECT
public:
    ScrollBar(QWidget *parent, Qt::Orientation t, QScrollBar *bar);
public slots:
    void onRangeChanged(int min, int max);
    void onValueSet(int value);
};

#define ANIMATED_SCROLL_CLAZZ_DEF \
    private:\
        ScrollBar* vBar{};\
        ScrollBar* hBar{};\
        QPropertyAnimation vBarAnimation{};\
        QPropertyAnimation hBarAnimation{};\
        int speed = 0;\
    protected:\
        void resizeEvent(QResizeEvent *event) override;\
        void wheelEvent(QWheelEvent *event) override;

#define ANIMATED_SCROLL_CLAZZ_CONSTRUCTOR\
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);\
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);\
    vBar = new ScrollBar(this,Qt::Vertical,verticalScrollBar());\
    vBarAnimation.setTargetObject(verticalScrollBar());\
    vBarAnimation.setPropertyName("value");\
    vBarAnimation.setDuration(300);\
    vBarAnimation.setEasingCurve(QEasingCurve::Linear);\
    hBar = new ScrollBar(this,Qt::Horizontal,horizontalScrollBar());\
    hBarAnimation.setTargetObject(horizontalScrollBar());\
    hBarAnimation.setPropertyName("value");\
    hBarAnimation.setDuration(300);\
    hBarAnimation.setEasingCurve(QEasingCurve::Linear);\
    connect(&hBarAnimation, &QPropertyAnimation::finished, this, [this](){\
        speed = 0;\
    });\
    connect(&vBarAnimation, &QPropertyAnimation::finished, this, [this](){\
        speed = 0;\
    });

#define ANIMATED_SCROLL_CLAZZ_OVERRIDES(CLAZZ, SUPER) \
    void CLAZZ::resizeEvent(QResizeEvent *event) {\
        hBar->setGeometry(0, height() - 8, width(), 7);\
        vBar->setGeometry(width() - 8, 0, 7, height());\
        SUPER::resizeEvent(event);\
    }\
    \
    void CLAZZ::wheelEvent(QWheelEvent *event) {\
        QPropertyAnimation* animation;\
        QScrollBar* bar;\
        if (event->modifiers() == Qt::ShiftModifier) {\
            animation = &hBarAnimation;\
            bar = hBar;\
        } else {\
            animation = &vBarAnimation;\
            bar = vBar;\
        }\
        if (animation->state() != QAbstractAnimation::Stopped) {\
            animation->stop();\
            animation->setEndValue(bar->value() - event->angleDelta().y() * ++speed);\
        } else {\
            animation->setEndValue(bar->value() - event->angleDelta().y());\
        }\
        animation->setStartValue(bar->value());\
        animation->start();\
    }
#endif //LIFERHYTHM_SCROLLBAR_H
