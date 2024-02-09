//
// Created by stupid_coder_jyy on 2024/2/8.
//

#ifndef LIFERHYTHM_STDIMGLABEL_H
#define LIFERHYTHM_STDIMGLABEL_H

#include <QLabel>
#include "StandardWidget.h"

class StdImgLabel : public QLabel, public StandardWidget{
    Q_OBJECT
protected:
    void mouseReleaseEvent(QMouseEvent *ev) override;

public:
    explicit StdImgLabel(QWidget* parent);
    void postParsing(Handlers &handlers, NBT *widgetTag) override;
signals:
    void sigActivated();
};


#endif //LIFERHYTHM_STDIMGLABEL_H
