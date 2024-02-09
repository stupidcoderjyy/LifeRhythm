//
// Created by stupid_coder_jyy on 2024/2/9.
//

#ifndef LIFERHYTHM_STDTEXTLABEL_H
#define LIFERHYTHM_STDTEXTLABEL_H

#include <QLabel>
#include "StandardWidget.h"

class StdTextLabel : public QLabel, public StandardWidget{
public:
    explicit StdTextLabel(QWidget* parent);
    void postParsing(Handlers &handlers, NBT *widgetTag) override;
};


#endif //LIFERHYTHM_STDTEXTLABEL_H
