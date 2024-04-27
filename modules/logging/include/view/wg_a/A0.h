//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_A0_H
#define LIFERHYTHM_A0_H

#include "RangeBar.h"
#include "TimeScale.h"

BEGIN_NAMESPACE(lr::log)

class TimeBarContainer : public VBarContainer {
    friend class A0;
public:
    explicit TimeBarContainer(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

class A0 : public RangeBar {
    Q_OBJECT
private:
    TimeScale* scale;
public:
    explicit A0(QWidget* parent = nullptr);
protected:
    BarItem *createRangeWidget() override;
    ScrollBar *createVerticalScrollBar() override;
    void assembleContainer() override;
    void updateContentSize() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_A0_H
