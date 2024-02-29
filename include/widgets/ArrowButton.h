//
// Created by stupid_coder_jyy on 2024/2/28.
//

#ifndef LIFERHYTHM_ARROWBUTTON_H
#define LIFERHYTHM_ARROWBUTTON_H

#include <QWidget>
#include <QPainterPath>

class ArrowButton : public QWidget {
    Q_OBJECT
public:
    enum Type {
        Up,
        Down,
        Left,
        Right
    };
private:
    Type type;
    Type activatedType;
    double ratioWidth;
    double ratioHeight;
    double scale;
    bool pressed;
    QPainterPath path;
    int points[6];
public:
    explicit ArrowButton(Type type = Down, QWidget* parent = nullptr);
    void setActivatedType(ArrowButton::Type t);
    void setRatioWidth(double ratioWidth);
    void setRatioHeight(double ratioHeight);
    void setRatio(double ratioWidth, double ratioHeight);
    void setScale(double scale);
    void setPressed(bool pressed = true);
signals:
    void sigActivated(bool pressed);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void updatePoints(Type type, bool update = true);
};


#endif //LIFERHYTHM_ARROWBUTTON_H
