//
// Created by stupid_coder_jyy on 2024/2/12.
//

#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QScrollArea>
#include "StandardWidget.h"
#include "ScrollBar.h"

BEGIN_LR

class CORE_API ScrollArea : public QScrollArea, public StandardWidget{
private:
    ScrollBar *vBar{};
    ScrollBar *hBar{};
public:
    explicit ScrollArea(QWidget* parent = nullptr, bool initInConstructor = true);
    ScrollBar* getVScrollBar();
    ScrollBar* getHScrollBar();
protected:
    void resizeEvent(QResizeEvent *event) override;
    virtual ScrollBar* createHorizontalScrollBar();
    virtual ScrollBar* createVerticalScrollBar();
};

END_LR

#endif //LIFERHYTHM_SCROLLAREA_H
