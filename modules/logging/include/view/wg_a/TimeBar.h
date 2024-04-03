//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_TIMEBAR_H
#define LIFERHYTHM_TIMEBAR_H

#include "RangeBar.h"
#include "TextLabel.h"
#include "Namespaces.h"

BEGIN_NAMESPACE(lr::log)

class TimeBarItem : public BarItem {
    friend class TimeBarContainer;
private:
    QColor colorBg;
    TextLabel* labelRange;
    TextLabel* labelInfo;
public:
    explicit TimeBarItem(QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void syncDataToWidget() override;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void init();
};

class TimeBarContainer : public VBarContainer {
    friend class TimeBar;
public:
    explicit TimeBarContainer(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
    void updateBarGeometry() override;
};

class TimeScale;

class TimeBar : public RangeBar {
    Q_OBJECT
private:
    TimeScale* scale;
public:
    explicit TimeBar(QWidget* parent = nullptr);
protected:
    BarItem *createRangeWidget() override;
    ScrollBar *createVerticalScrollBar() override;
    void assembleContainer() override;
    void updateContainerSize() override;
};

class TimeScale : public QWidget {
private:
    double vpp;
public:
    explicit TimeScale(QWidget* parent = nullptr);
    void setVpp(double vpp);
protected:
    void paintEvent(QPaintEvent *event) override;
};

END_NAMESPACE

#endif //LIFERHYTHM_TIMEBAR_H
