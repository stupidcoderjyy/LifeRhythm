//
// Created by stupid_coder_jyy on 2024/3/20.
//

#include "GridLayout.h"

GridLayout::GridLayout(QWidget *parent, int maxColumns): QGridLayout(parent),
        curColumn(), curRow(), maxColumns(maxColumns) {
}

void GridLayout::appendWidget(QWidget *widget) {
    auto align = curColumn < columnAlign.count() ? columnAlign.at(curColumn) : Qt::Alignment{};
    addWidget(widget, curRow, curColumn, align);
    if (curColumn == maxColumns - 1) {
        curColumn = 0;
        curRow++;
    } else {
        curColumn++;
    }
}
