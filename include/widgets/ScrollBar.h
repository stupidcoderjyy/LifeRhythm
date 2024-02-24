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
        float speed = 1;\
        bool up{};\
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
        speed = 1;\
    });\
    connect(&vBarAnimation, &QPropertyAnimation::finished, this, [this](){\
        speed = 1;\
    });

#define ANIMATED_SCROLL_CLAZZ_OVERRIDES_RESIZE(CLAZZ, SUPER) \
    void CLAZZ::resizeEvent(QResizeEvent *event) {\
        hBar->setGeometry(0, height() - 8, width(), 7);\
        vBar->setGeometry(width() - 8, 0, 7, height());\
        SUPER::resizeEvent(event);\
    }

#define ANIMATED_SCROLL_CLAZZ_OVERRIDES_WHEEL(CLAZZ) \
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
    bool isUp = event->angleDelta().y() > 0;\
    if (animation->state() != QAbstractAnimation::Stopped) {\
        animation->stop();\
        if (isUp != up) {\
            animation->setEndValue(bar->value() - event->angleDelta().y());\
            speed = 1;\
        } else {\
            speed += 0.2f;\
            animation->setEndValue(bar->value() - qRound((double)event->angleDelta().y() * qMin(speed, 1.6f)));\
        }\
    } else {\
        animation->setEndValue(bar->value() - event->angleDelta().y());\
    }\
    up = isUp;\
    animation->setStartValue(bar->value());\
    animation->start();\
}

#define ANIMATED_SCROLL_CLAZZ_OVERRIDES(CLAZZ, SUPER) \
    ANIMATED_SCROLL_CLAZZ_OVERRIDES_WHEEL(CLAZZ)      \
    ANIMATED_SCROLL_CLAZZ_OVERRIDES_RESIZE(CLAZZ, SUPER)

#endif //LIFERHYTHM_SCROLLBAR_H
