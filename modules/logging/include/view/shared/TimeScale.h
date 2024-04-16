//
// Created by stupid_coder_jyy on 2024/4/15.
//

#ifndef LIFERHYTHM_TIMESCALE_H
#define LIFERHYTHM_TIMESCALE_H

#include "Namespaces.h"
#include <QWidget>

BEGIN_NAMESPACE(lr::log)

class TimeScale : public QWidget {
private:
    double vpp;
public:
    explicit TimeScale(QWidget* parent = nullptr);
    void setVpp(double vpp);
protected:
    void paintEvent(QPaintEvent *event) override;
};

END_NAMESPACE

#endif //LIFERHYTHM_TIMESCALE_H
