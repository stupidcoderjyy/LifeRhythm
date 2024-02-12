//
// Created by stupid_coder_jyy on 2024/2/12.
//

#ifndef LIFERHYTHM_SCROLLBAR_H
#define LIFERHYTHM_SCROLLBAR_H

#include <QScrollBar>
#include <QPropertyAnimation>

class ScrollBar : public QScrollBar{
    Q_OBJECT
public:
    ScrollBar(QWidget *parent, Qt::Orientation t, QScrollBar *bar);
public slots:
    void onRangeChanged(int min, int max);
    void onValueSet(int value);
};


#endif //LIFERHYTHM_SCROLLBAR_H
