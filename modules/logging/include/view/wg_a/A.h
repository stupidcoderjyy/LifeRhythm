//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_A_H
#define LIFERHYTHM_A_H

#include "TimeBar.h"
#include "TabBar.h"

BEGIN_NAMESPACE(lr::log)

class PeriodDataLoader;

class A : public TabWidget {
    Q_OBJECT
private:
    TimeBar* bar;
public:
    explicit A(QWidget* parent = nullptr);
protected:
    void onTabCreated() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_A_H
