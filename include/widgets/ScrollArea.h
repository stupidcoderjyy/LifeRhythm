//
// Created by stupid_coder_jyy on 2024/2/12.
//

#ifndef LIFERHYTHM_SCROLLAREA_H
#define LIFERHYTHM_SCROLLAREA_H

#include <QScrollArea>
#include "StandardWidget.h"
#include "ScrollBar.h"

class ScrollArea : public QScrollArea, public StandardWidget{
private:
    ScrollBar* vBar{};
    ScrollBar* hBar{};
    QPropertyAnimation vBarAnimation{};
    QPropertyAnimation hBarAnimation{};
    int speed = 0;
public:
    explicit ScrollArea(QWidget* parent = nullptr);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};


#endif //LIFERHYTHM_SCROLLAREA_H
