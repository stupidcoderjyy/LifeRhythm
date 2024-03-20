//
// Created by stupid_coder_jyy on 2024/3/20.
//

#include "PeriodTypeIcon.h"
#include "PeriodType.h"

PeriodTypeIcon::PeriodTypeIcon(QWidget *parent): Widget(parent) {
    setFixedSize(20, 20);
}

void PeriodTypeIcon::paintEvent(QPaintEvent *event) {
    if (!wData) {
        return;
    }
    int fw = 2, dfw = fw << 1, d = 5;
    QPainter painter(this);
    auto* type = wData->cast<PeriodType>();
    if (type->getChildren().empty()) {
        painter.fillRect(rect(), QColor(Styles::GRAY_TEXT_0));
        painter.fillRect(fw, fw, width() - dfw, height() - dfw, type->color);
    } else {
        int dw = width() - d, dh = height() - d;
        painter.fillRect(0, 0, dw, dh, QColor(Styles::GRAY_TEXT_0));
        painter.fillRect(fw, fw, dw - dfw, dh - dfw, type->color);
        painter.fillRect(d, d, dw, dh, QColor(Styles::GRAY_TEXT_0));
        painter.fillRect(d + fw, d + fw, dw - dfw, dh - dfw, type->color);
    }
}