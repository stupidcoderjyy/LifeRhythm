//
// Created by stupid_coder_jyy on 2024/2/8.
//

#ifndef LIFERHYTHM_STDIMGBUTTON_H
#define LIFERHYTHM_STDIMGBUTTON_H

#include "StdImgLabel.h"

class StdImgButton : public StdImgLabel{
private:
    QString qssHovered, qssSelected, qssNormal;
public:
    explicit StdImgButton(QWidget* parent);
protected:
    void enterEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *event) override;
};


#endif //LIFERHYTHM_STDIMGBUTTON_H
