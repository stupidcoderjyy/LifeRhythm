//
// Created by stupid_coder_jyy on 2024/2/23.
//

#ifndef LIFERHYTHM_DRAGWIDGET_H
#define LIFERHYTHM_DRAGWIDGET_H

#include <QWidget>

class WidgetDragger : public QObject{
    Q_OBJECT
public:
    enum Orientation {
        Left,
        Top,
        Right,
        Bottom,
        Center
    };
private:
    QWidget* target{};
    bool allowDrag[4]{};
    QPoint startPos{};
    Orientation hOrientation = Center;
    Orientation vOrientation = Center;
    bool mousePressed{};
public:
    explicit WidgetDragger(QObject* parent = nullptr);
    void setEnabled(Orientation o, bool enabled = true);
    void setTarget(QWidget* target);
    void onMousePressed(QMouseEvent* evt);
    void onMouseReleased();
    void onMouseMoved(QMouseEvent* evt);
protected:
    virtual int dragDistance(Orientation o, int delta);
    virtual bool shouldDrag(Orientation o);
    virtual void checkHor();
    virtual void checkVer();
    virtual void applyDrag(Orientation o, int delta);
};

#endif //LIFERHYTHM_DRAGWIDGET_H
