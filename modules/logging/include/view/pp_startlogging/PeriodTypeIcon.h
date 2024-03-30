//
// Created by stupid_coder_jyy on 2024/3/20.
//

#ifndef LIFERHYTHM_PERIODTYPEICON_H
#define LIFERHYTHM_PERIODTYPEICON_H

#include "Widget.h"

class PeriodTypeIcon : public Widget {
    friend class PeriodTypeTreeItem;
public:
    explicit PeriodTypeIcon(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif //LIFERHYTHM_PERIODTYPEICON_H
