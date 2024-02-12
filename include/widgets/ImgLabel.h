//
// Created by stupid_coder_jyy on 2024/2/8.
//

#ifndef LIFERHYTHM_IMGLABEL_H
#define LIFERHYTHM_IMGLABEL_H

#include <QLabel>
#include "StandardWidget.h"

class ImgLabel : public QLabel, public StandardWidget{
    Q_OBJECT
protected:
    void mouseReleaseEvent(QMouseEvent *ev) override;
public:
    explicit ImgLabel(QWidget* parent);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void onStateRespondersParsing(Handlers &responders, NBT *stateTag) override;
signals:
    void sigActivated();
};


#endif //LIFERHYTHM_IMGLABEL_H
