//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_PERIODWIDGET_H
#define LIFERHYTHM_PERIODWIDGET_H

#include "RangeBar.h"
#include "TextLabel.h"

class PeriodWidget : public RangeWidget {
private:
    TextLabel* labelRange;
    TextLabel* labelInfo;
public:
    static void mainInit();
    explicit PeriodWidget(QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void syncDataToWidget() override;
private:
    void init();
};

#endif //LIFERHYTHM_PERIODWIDGET_H
