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
    ScrollBar(QWidget *parent, Qt::Orientation t);
public slots:
    void onRangeChanged(int min, int max);
    void onValueSet(int value);
};

#define SCROLL_CLAZZ_DEF \
    private:\
        ScrollBar* vBar{};\
        ScrollBar* hBar{};\
    protected:\
        void resizeEvent(QResizeEvent *event) override;

#define SCROLL_CLAZZ_CONSTRUCTOR\
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);\
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);\
    vBar = new ScrollBar(this,Qt::Vertical);\
    hBar = new ScrollBar(this,Qt::Horizontal);\
    connect(verticalScrollBar(), &QScrollBar::valueChanged, vBar, &ScrollBar::onValueSet);\
    connect(vBar, &QScrollBar::valueChanged, [this](int v){verticalScrollBar()->setValue(v);});\
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, vBar, &ScrollBar::onRangeChanged);\
    connect(horizontalScrollBar(), &QScrollBar::valueChanged, hBar, &ScrollBar::onValueSet);\
    connect(hBar, &QScrollBar::valueChanged, [this](int v){horizontalScrollBar()->setValue(v);});\
    connect(horizontalScrollBar(), &QScrollBar::rangeChanged, hBar, &ScrollBar::onRangeChanged);

#define SCROLL_CLAZZ_OVERRIDES(CLAZZ, SUPER) \
    void CLAZZ::resizeEvent(QResizeEvent *event) {\
        hBar->setGeometry(0, height() - 8, width(), 7);\
        vBar->setGeometry(width() - 8, 0, 7, height());\
        SUPER::resizeEvent(event);\
    }

#endif //LIFERHYTHM_SCROLLBAR_H
