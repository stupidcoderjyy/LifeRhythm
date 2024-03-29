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
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif //LIFERHYTHM_SCROLLBAR_H
