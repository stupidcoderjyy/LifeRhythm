//
// Created by stupid_coder_jyy on 2024/4/15.
//

#ifndef LIFERHYTHM_WEEKVIEW_H
#define LIFERHYTHM_WEEKVIEW_H

#include "RangeBar.h"
#include "Namespaces.h"
#include "AbstractSlotsDrawer.h"
#include "TimeScale.h"
#include <QDate>

BEGIN_NAMESPACE(lr::log)

class WeekViewContainer : public VBarContainer {
public:
    explicit WeekViewContainer(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

class WeekView : public RangeBar {
    Q_OBJECT
private:
    TimeScale* scale;
public:
    explicit WeekView(QWidget* parent = nullptr);
protected:
    void assembleContainer() override;
    void updateContainerSize() override;
    BarItem *createRangeWidget() override;
    ScrollBar *createVerticalScrollBar() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_WEEKVIEW_H
