//
// Created by stupid_coder_jyy on 2024/2/12.
//

#include "widgets/ScrollArea.h"
#include "NBT.h"
#include "Styles.h"
#include <QDebug>
#include <QWheelEvent>

ScrollArea::ScrollArea(QWidget *parent):QScrollArea(parent),StandardWidget() {
    setFrameShape(QFrame::NoFrame);
    setWidgetResizable(true);
    viewport()->setObjectName("vp");
    viewport()->setStyleSheet(qss_t("vp", bg(Styles::CLEAR)));
    setObjectName("sa");
    setStyleSheet(qss_t("sa", bg(Styles::CLEAR)));
    SCROLL_CLAZZ_CONSTRUCTOR
}

SCROLL_CLAZZ_OVERRIDES(ScrollArea, QScrollArea)