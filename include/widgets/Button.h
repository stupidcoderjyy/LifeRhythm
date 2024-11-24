//
// Created by stupid_coder_jyy on 2024/3/12.
//

#ifndef LR_BUTTON_H
#define LR_BUTTON_H

#include "Label.h"

class Button : public Label {
    Q_OBJECT
public:
    enum Mode {
        Click,
        Select,
        SelectClick
    };
protected:
    bool running;
    bool selected;
    bool hasStyle;
    bool activatedOnPress;
    bool hasImg;
    bool enabled;
    Mode type;
    QPixmap imgNormal, imgDisabled;
public:
    explicit Button(QWidget* parent = nullptr, bool initInConstructor = true);
    void setButtonText(const QString& text);
    void setButtonImg(const QPixmap& p1, const QPixmap& p2 = {});
    void setButtonEnabled(bool e = true);
    inline void setButtonMode(Mode type);
    void setButtonStyleEnabled(bool enabled);
    inline void setActivateOnPress(bool aop);
    virtual void setSelected(bool selected);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    signals:
        void sigActivated();
    void sigSelected(bool selected);
protected:
    void enterEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *event) override;
private:
    void handleButtonActivate(QMouseEvent* ev);
};

inline void Button::setButtonMode(Mode t) {
    type = t;
}

inline void Button::setActivateOnPress(bool aop) {
    activatedOnPress = aop;
}

#endif //BUTTON_H
