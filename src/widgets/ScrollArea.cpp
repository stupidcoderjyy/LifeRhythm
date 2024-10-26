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
    viewport()->setStyleSheet(qss_target("vp", bg(Styles::BLACK->rgbHex)));
    setObjectName("sa");
    setStyleSheet(qss_target("sa", bg(Styles::BLACK->rgbHex)));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

ScrollBar *ScrollArea::getVScrollBar() {
    if (!vBar) {
        vBar = createVerticalScrollBar();
    }
    return vBar;
}

ScrollBar *ScrollArea::getHScrollBar() {
    if (!hBar) {
        hBar = createHorizontalScrollBar();
    }
    return hBar;
}

ScrollBar* ScrollArea::createHorizontalScrollBar() {
    auto* b = new ScrollBar(this, Qt::Horizontal);
    connect(horizontalScrollBar(), &QScrollBar::valueChanged, b, &ScrollBar::onValueSet);
    connect(b, &QScrollBar::valueChanged, [this](int v){horizontalScrollBar()->setValue(v);});
    connect(horizontalScrollBar(), &QScrollBar::rangeChanged, b, &ScrollBar::onRangeChanged);
    return b;
}

ScrollBar* ScrollArea::createVerticalScrollBar() {
    auto* b = new ScrollBar(this,Qt::Vertical);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, b, &ScrollBar::onValueSet);
    connect(b, &QScrollBar::valueChanged, [this](int v){verticalScrollBar()->setValue(v);});
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, b, &ScrollBar::onRangeChanged);
    return b;
}

void ScrollArea::resizeEvent(QResizeEvent *event) {
    getHScrollBar()->setGeometry(0, height() - 8, width(), 7);
    getVScrollBar()->setGeometry(width() - 8, 0, 7, height());
    QScrollArea::resizeEvent(event);
}