//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_PERIOD_H
#define LIFERHYTHM_PERIOD_H

#include "RangeBar.h"
#include "Namespaces.h"

BEGIN_NAMESPACE(lr::log)

class Period : public BarData {
    Q_OBJECT
public:
    QString info;
    Period(int begin, int end, QString info);
    Period();
};

END_NAMESPACE

#endif //LIFERHYTHM_PERIOD_H
