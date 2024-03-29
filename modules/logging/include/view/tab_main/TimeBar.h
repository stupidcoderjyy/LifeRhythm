//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_TIMEBAR_H
#define LIFERHYTHM_TIMEBAR_H

#include "RangeBar.h"
#include "TextLabel.h"

class TimeBarItem : public RangeBarItem {
private:
    TextLabel* labelRange;
    TextLabel* labelInfo;
public:
    explicit TimeBarItem(QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void syncDataToWidget() override;
private:
    void init();
};

class TimeBar : public RangeBar {
    Q_OBJECT
public:
    explicit TimeBar(QWidget* parent = nullptr);
protected:
    RangeBarItem *createRangeWidget() override;
};


#endif //LIFERHYTHM_TIMEBAR_H
