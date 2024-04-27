//
// Created by stupid_coder_jyy on 2024/3/20.
//

#include "UsageTypeIcon.h"
#include "UsageType.h"

USING_NAMESPACE(lr::log)

UsageTypeIcon::UsageTypeIcon(QWidget *parent): Widget(parent) {
    setFixedSize(20, 20);
}

void UsageTypeIcon::paintEvent(QPaintEvent *event) {
    if (!wData) {
        return;
    }
    int fw = 2, dfw = fw << 1, d = 5;
    QPainter painter(this);
    auto* node = wData->cast<TreeNode>();
    auto* type = node->nodeData()->cast<UsageType>();
    if (node->getChildren().empty()) {
        painter.fillRect(rect(), QColor(Styles::GRAY_TEXT_0->rgbHex));
        painter.fillRect(fw, fw, width() - dfw, height() - dfw, type->getColor());
    } else {
        int dw = width() - d, dh = height() - d;
        painter.fillRect(0, 0, dw, dh, QColor(Styles::GRAY_TEXT_0->rgbHex));
        painter.fillRect(fw, fw, dw - dfw, dh - dfw, type->getColor());
        painter.fillRect(d, d, dw, dh, QColor(Styles::GRAY_TEXT_0->rgbHex));
        painter.fillRect(d + fw, d + fw, dw - dfw, dh - dfw, type->getColor());
    }
}