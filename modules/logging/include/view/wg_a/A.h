//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_A_H
#define LIFERHYTHM_A_H

#include "A0.h"
#include "TabBar.h"

BEGIN_NAMESPACE(lr::log)

class A : public TabWidget {
    Q_OBJECT
private:
    A0* bar;
public:
    explicit A(QWidget* parent = nullptr);
protected:
    void onTabCreated() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_A_H
