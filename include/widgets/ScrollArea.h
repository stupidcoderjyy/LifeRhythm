//
// Created by stupid_coder_jyy on 2024/2/12.
//

#ifndef LIFERHYTHM_SCROLLAREA_H
#define LIFERHYTHM_SCROLLAREA_H

#include <QScrollArea>
#include "StandardWidget.h"
#include "ScrollBar.h"

class ScrollArea : public QScrollArea, public StandardWidget{
public:
    explicit ScrollArea(QWidget* parent = nullptr);
ANIMATED_SCROLL_CLAZZ_DEF
};


#endif //LIFERHYTHM_SCROLLAREA_H
