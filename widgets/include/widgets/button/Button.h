//
// Created by stupid_coder_jyy on 2024/3/12.
//

#ifndef BUTTON_H
#define BUTTON_H

#include "Label.h"

BEGIN_LR

class Button : public Label {
    Q_OBJECT
public:
    static QString QSS_NORMAL;
    static QString QSS_HOVERED;
    static QString QSS_PRESSED;
    static QString QSS_SELECTED;
    static QString QSS_SELECTED_DISABLED;
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
    bool hasFocus;
    Mode type;
    QPixmap imgNormal, imgDisabled;
public:
    explicit Button(QWidget* parent = nullptr, bool initInConstructor = true);
    void setButtonText(const QString& text);
    void setButtonImg(const QPixmap& p1, const QPixmap& p2 = {});
    void setButtonStyleEnabled(bool enabled);
    virtual void setSelected(bool selected);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void setWidgetEnabled(bool e = true) override;
    inline void setActivateOnPress(bool aop);
    inline void setButtonMode(Mode type);
    inline void setHasFocus(bool hasFocus);
    static void mainInit();
signals:
    void sigActivated();
    void sigSelected(bool selected);
protected:
    void enterEvent(QEnterEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *event) override;
    virtual void handleButtonActivate(QMouseEvent* ev);
};

inline void Button::setButtonMode(Mode t) {
    type = t;
}

inline void Button::setActivateOnPress(bool aop) {
    activatedOnPress = aop;
}

inline void Button::setHasFocus(bool hf) {
    hasFocus = hf;
}

END_LR

#endif //BUTTON_H
