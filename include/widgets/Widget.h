//
// Created by stupid_coder_jyy on 2024/1/16.
//

#ifndef LIFERHYTHM_WIDGET_H
#define LIFERHYTHM_WIDGET_H

#include <QWidget>
#include "StandardWidget.h"
#include "Styles.h"
#include <QPainter>

class CORE_API Widget : public QWidget, public StandardWidget {
public:
    explicit Widget(QWidget* parent = nullptr);
PAINT_QSS
};
#endif //LIFERHYTHM_WIDGET_H
