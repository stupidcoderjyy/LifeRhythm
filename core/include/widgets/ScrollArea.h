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
    ScrollBar *vBar{};
    ScrollBar *hBar{};
public:
    explicit ScrollArea(QWidget* parent = nullptr);
    ScrollBar* getVScrollBar();
    ScrollBar* getHScrollBar();
protected:
    void resizeEvent(QResizeEvent *event) override;
    virtual ScrollBar* createHorizontalScrollBar();
    virtual ScrollBar* createVerticalScrollBar();
};


#endif //LIFERHYTHM_SCROLLAREA_H
