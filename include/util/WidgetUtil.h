//
// Created by stupid_coder_jyy on 2024/2/11.
//

#ifndef LIFERHYTHM_WIDGETUTIL_H
#define LIFERHYTHM_WIDGETUTIL_H

#include <QWidget>
#include <QMouseEvent>

bool isHovered(QWidget* widget, QMouseEvent* evt);
bool isHovered(QWidget* widget, const QPoint& pos);

#endif //LIFERHYTHM_WIDGETUTIL_H
