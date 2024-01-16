//
// Created by stupid_coder_jyy on 2024/1/16.
//

#ifndef LIFERHYTHM_STDWIDGET_H
#define LIFERHYTHM_STDWIDGET_H

#include <QWidget>
#include "StandardWidget.h"

class StdWidget : public QWidget, public StandardWidget{
public:
    explicit StdWidget(QWidget* parent);
};
#endif //LIFERHYTHM_STDWIDGET_H
