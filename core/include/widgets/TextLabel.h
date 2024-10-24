//
// Created by stupid_coder_jyy on 2024/2/9.
//

#ifndef LIFERHYTHM_TEXTLABEL_H
#define LIFERHYTHM_TEXTLABEL_H

#include <QLabel>
#include "StandardWidget.h"

class CORE_API TextLabel : public QLabel, public StandardWidget{
public:
    explicit TextLabel(QWidget* parent = nullptr);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void onStateRespondersParsing(Handlers &responders, NBT *stateTag) override;
protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
};


#endif //LIFERHYTHM_TEXTLABEL_H
