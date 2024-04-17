//
// Created by stupid_coder_jyy on 2024/4/17.
//

#ifndef LIFERHYTHM_B0_H
#define LIFERHYTHM_B0_H

#include "Calendar.h"
#include "Namespaces.h"

BEGIN_NAMESPACE(lr::log)

class B0 : public Calendar {
public:
    explicit B0(QWidget* parent = nullptr);
protected:
    void init() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_B0_H
