//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_TIMEBAR_H
#define LIFERHYTHM_TIMEBAR_H

#include "RangeBar.h"

class TimeBar : public RangeBar {
    Q_OBJECT
public:
    explicit TimeBar(QWidget* parent = nullptr);
protected:
    RangeWidget *createRangeWidget() override;
};


#endif //LIFERHYTHM_TIMEBAR_H
