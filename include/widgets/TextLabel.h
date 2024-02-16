//
// Created by stupid_coder_jyy on 2024/2/9.
//

#ifndef LIFERHYTHM_TEXTLABEL_H
#define LIFERHYTHM_TEXTLABEL_H

#include <QLabel>
#include "StandardWidget.h"

class TextLabel : public QLabel, public StandardWidget{
public:
    explicit TextLabel(QWidget* parent);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void onStateRespondersParsing(Handlers &responders, NBT *stateTag) override;
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
};


#endif //LIFERHYTHM_TEXTLABEL_H
