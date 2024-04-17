//
// Created by stupid_coder_jyy on 2024/4/15.
//

#ifndef LIFERHYTHM_B_H
#define LIFERHYTHM_B_H

#include "TabBar.h"
#include "Namespaces.h"
#include "B4.h"
#include "B3.h"

BEGIN_NAMESPACE(lr::log)

class B : public TabWidget {
    Q_OBJECT
private:
    B3* wvTitle;
    B4* wv;
public:
    explicit B(QWidget* parent = nullptr);
protected:
    void onTabCreated() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_B_H
