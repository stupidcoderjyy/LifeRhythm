//
// Created by stupid_coder_jyy on 2024/2/11.
//

#ifndef LIFERHYTHM_WIDGETUTIL_H
#define LIFERHYTHM_WIDGETUTIL_H

#include <QWidget>

#include "Plugin.h"

CORE_API bool isHovered(QWidget* widget, QMouseEvent* evt);
CORE_API bool isHovered(QWidget* widget, const QPoint& pos);
CORE_API bool isMouseHovered(QWidget* widget);
CORE_API QPoint getGlobalPos(QWidget* widget);
CORE_API void moveWidget(QWidget* widget, const QPoint& pos);
CORE_API void moveWidget(QWidget* widget, int x, int y);
CORE_API void switchSingleLayoutWidget(QLayout* layout, QWidget* pre, QWidget* cur);

CORE_API void fillRoundedRect(QPainter& painter, const QRect& rect, int radius, const QBrush& color);

#endif //LIFERHYTHM_WIDGETUTIL_H
