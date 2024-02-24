//
// Created by stupid_coder_jyy on 2024/2/24.
//

#ifndef LIFERHYTHM_TIMEBAR_H
#define LIFERHYTHM_TIMEBAR_H

#include "RangeBar.h"
#include "WidgetFactory.h"
#include "TextLabel.h"
#include "Period.h"

class TimeBar : public RangeBar{
private:
    static WidgetFactory* factory;
public:
    static void init();
    explicit TimeBar(QWidget* parent = nullptr);
protected:
    RangeWidget *createRangeWidget() override;
};

class TimeBarRangeWidget : public RangeWidget {
private:
    TextLabel* labelRange{};
    TextLabel* labelInfo{};
public:
    explicit TimeBarRangeWidget(QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void syncDataToWidget() override;
    void syncWidgetToData() override;
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};


#endif //LIFERHYTHM_TIMEBAR_H
