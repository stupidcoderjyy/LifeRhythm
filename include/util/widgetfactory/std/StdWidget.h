//
// Created by stupid_coder_jyy on 2024/1/16.
//

#ifndef LIFERHYTHM_STDWIDGET_H
#define LIFERHYTHM_STDWIDGET_H

#include <QWidget>
#include "StandardWidget.h"
#include "Styles.h"

class StdWidget : public QWidget, public StandardWidget{
public:
    explicit StdWidget(QWidget* parent);
PAINT_QSS
};
#endif //LIFERHYTHM_STDWIDGET_H
