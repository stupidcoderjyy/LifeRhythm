//
// Created by stupid_coder_jyy on 2024/4/15.
//

#ifndef LIFERHYTHM_B4_H
#define LIFERHYTHM_B4_H

#include "RangeBar.h"
#include "Namespaces.h"
#include "SlotsDrawer.h"
#include "TimeScale.h"
#include <QDate>

BEGIN_NAMESPACE(lr::log)

class WeekViewContainer : public VBarContainer {
public:
    explicit WeekViewContainer(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

class B4 : public RangeBar {
    Q_OBJECT
private:
    TimeScale* scale;
public:
    explicit B4(QWidget* parent = nullptr);
protected:
    void assembleContainer() override;
    void updateContentSize() override;
    BarItem *createRangeWidget() override;
    ScrollBar *createVerticalScrollBar() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_B4_H
