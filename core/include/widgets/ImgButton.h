//
// Created by stupid_coder_jyy on 2024/2/8.
//

#ifndef LIFERHYTHM_IMGBUTTON_H
#define LIFERHYTHM_IMGBUTTON_H

#include "ImgLabel.h"

class ImgButton : public ImgLabel{
private:
    QString qssHovered, qssNormal, qssPressed;
public:
    explicit ImgButton(QWidget* parent);
protected:
    void enterEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *event) override;
};


#endif //LIFERHYTHM_IMGBUTTON_H
