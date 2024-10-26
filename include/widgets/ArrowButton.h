//
// Created by stupid_coder_jyy on 2024/2/28.
//

#ifndef LIFERHYTHM_ARROWBUTTON_H
#define LIFERHYTHM_ARROWBUTTON_H

#include "Widget.h"

class CORE_API ArrowButton : public Widget {
    Q_OBJECT
public:
    enum Type {
        Up,
        Down,
        Left,
        Right,
        None
    };
private:
    static QMap<QString, Type> directions;
    Type type;
    Type activatedType;
    double ratioWidth;
    double ratioHeight;
    double scale;
    bool pressed;
    int points[6];
public:
    static void mainInit();
    explicit ArrowButton(QWidget* parent = nullptr);
    void setActivatedType(ArrowButton::Type t);
    void setBaseType(ArrowButton::Type t);
    void setRatioWidth(double ratioWidth);
    void setRatioHeight(double ratioHeight);
    void setRatio(double ratioWidth, double ratioHeight);
    void setScale(double scale);
    void setPressed(bool pressed = true);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
signals:
    void sigActivated(bool pressed);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void updatePoints(Type type, bool update = true);
};

#endif //LIFERHYTHM_ARROWBUTTON_H
