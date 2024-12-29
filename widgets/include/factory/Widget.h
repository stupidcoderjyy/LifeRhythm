//
// Created by stupid_coder_jyy on 2024/1/16.
//

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "StandardWidget.h"
#include "Styles.h"
#include <QPainter>

BEGIN_LR

class CORE_API Widget : public QWidget, public StandardWidget {
public:
    explicit Widget(QWidget* parent = nullptr, bool initInConstructor = true);
PAINT_QSS
};

END_LR

#endif //LIFERHYTHM_WIDGET_H
