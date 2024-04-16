//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_TIMEBAR_H
#define LIFERHYTHM_TIMEBAR_H

#include "RangeBar.h"
#include "TimeScale.h"

BEGIN_NAMESPACE(lr::log)

class TimeBarContainer : public VBarContainer {
    friend class TimeBar;
public:
    explicit TimeBarContainer(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

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

END_NAMESPACE

#endif //LIFERHYTHM_TIMEBAR_H
