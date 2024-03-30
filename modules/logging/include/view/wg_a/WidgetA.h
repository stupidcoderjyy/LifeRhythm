//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_WIDGETA_H
#define LIFERHYTHM_WIDGETA_H

#include "TimeBar.h"
#include "TabBar.h"
#include "SelectableListData.h"

BEGIN_NAMESPACE(lr::log)

class PeriodDataLoader;

class WidgetA : public TabWidget {
    Q_OBJECT
private:
    TimeBar* bar;
    PeriodDataLoader* loader;
public:
    explicit WidgetA(QWidget* parent = nullptr);
protected:
    void onTabCreated() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_WIDGETA_H
