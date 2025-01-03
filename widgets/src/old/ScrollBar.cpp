//
// Created by stupid_coder_jyy on 2024/2/12.
//

#include "ScrollBar.h"

USING_LR

ScrollBar::ScrollBar(QWidget *parent, Qt::Orientation t): QScrollBar(parent) {
    setOrientation(t);
    setRange(0,0);
    hide();
    if (t == Qt::Vertical) {
        setFixedWidth(7);
        setStyleSheet("QScrollBar:vertical{background-color:transparent;padding-top:10px;padding-bottom:10px;}"
                      "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background-color:transparent;}"
                      "QScrollBar::handle:vertical{background:rgba(150,150,150,0.3);}"
                      "QScrollBar::handle:vertical:hover,QScrollBar::handle:vertical:pressed{background:rgb(100,100,100);}"
                      "QScrollBar::sub-line:vertical{background:transparent;}"
                      "QScrollBar::add-line:vertical{background:transparent;}");
    } else {
        setFixedHeight(7);
        setStyleSheet("QScrollBar:horizontal{background-color:transparent;padding-left:10px;padding-right:10px;}"
                      "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background-color:transparent;}"
                      "QScrollBar::handle:horizontal{background:rgba(150,150,150,0.3);}"
                      "QScrollBar::handle:horizontal:hover,QScrollBar::handle:horizontal:pressed{background:rgb(100,100,100);}"
                      "QScrollBar::sub-line:horizontal{background:transparent;}"
                      "QScrollBar::add-line:horizontal{background:transparent;}");
    }
    setContextMenuPolicy(Qt::NoContextMenu);
}

void ScrollBar::onRangeChanged(int min, int max) {
    setMinimum(min);
    setRange(0, max);
    setPageStep(static_cast<int>(0.75 * (height() + max)));
    if (max <= 0) {
        hide();
    } else if (isEnabled()) {
        show();
    }
}

void ScrollBar::onValueSet(int value) {
    setValue(value);
}

void ScrollBar::mousePressEvent(QMouseEvent *event) {
    QScrollBar::mousePressEvent(event);
    event->accept();
}
