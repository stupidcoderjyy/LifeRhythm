//
// Created by stupid_coder_jyy on 2024/2/12.
//

#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <QScrollBar>
#include <QWheelEvent>
#include "Namespaces.h"
#include "Plugin.h"

BEGIN_LR

class CORE_API ScrollBar : public QScrollBar{
    Q_OBJECT
public:
    ScrollBar(QWidget *parent, Qt::Orientation t);
public slots:
    void onRangeChanged(int min, int max);
    void onValueSet(int value);
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

END_LR

#endif //SCROLLBAR_H
