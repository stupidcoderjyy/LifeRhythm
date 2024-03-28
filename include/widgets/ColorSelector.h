//
// Created by stupid_coder_jyy on 2024/3/21.
//

#ifndef LIFERHYTHM_COLORSELECTOR_H
#define LIFERHYTHM_COLORSELECTOR_H

#include "SelectableListWidget.h"
#include "TextLabel.h"
#include <QWidget>

class VColorBar : public Widget {
    Q_OBJECT
private:
    bool pressed;
    QPixmap* barImg;
    int val;
    int step;
public:
    explicit VColorBar(QWidget* parent = nullptr);
    void selectColor(const QColor& c);
signals:
    void sigColorSelected(const QColor& color);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    QColor getColor() const;
    void onColorSelected(int v);
};

class ColorBarImageGenerator {
private:
    QPixmap* img;
    int pos;
    int step;
    int barWidth;
    bool isVertical;
public:
    ColorBarImageGenerator();
    void setBarData(bool vertical = false, int step = 1, int barWidth = 20);
    void outputImg(const QString& path);
    ~ColorBarImageGenerator();
private:
    void updateAll();
};

class ColorIcon : public Widget {
private:
    int bdWidth;
    QColor bdColor;
public:
    explicit ColorIcon(QWidget* parent = nullptr);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void setBorderWidth(int w);
    void setBorderColor(QColor c);
protected:
    void paintEvent(QPaintEvent *event) override;
};

class DefaultColorsListItem : public SelectableListItem {
private:
    ColorIcon* icon;
    TextLabel* labelName;
public:
    explicit DefaultColorsListItem(QWidget* parent = nullptr);
    void syncDataToWidget() override;
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
private:
    void init();
};

class DefaultColorsList : public SelectableListWidget {
    Q_OBJECT
public:
    explicit DefaultColorsList(QWidget* parent = nullptr);
protected:
    SelectableListItem *createRowItem() override;
};

class HueSelector : public Widget {
    Q_OBJECT
private:
    VColorBar* bar;
public:
    explicit HueSelector(QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
signals:
    void sigColorSelected(const QColor& color);
private:
    void init();
};

#endif //LIFERHYTHM_COLORSELECTOR_H
