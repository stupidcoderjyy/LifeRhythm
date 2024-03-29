//
// Created by stupid_coder_jyy on 2024/3/20.
//

#ifndef LIFERHYTHM_GRIDLAYOUT_H
#define LIFERHYTHM_GRIDLAYOUT_H

#include <QGridLayout>

class GridLayout : public QGridLayout {
    friend class WidgetFactory;
private:
    int curColumn;
    int curRow;
    int maxColumns;
    QVector<Qt::Alignment> columnAlign;
public:
    explicit GridLayout(QWidget* parent, int maxColumns = 1);
    void appendWidget(QWidget* widget);
};


#endif //LIFERHYTHM_GRIDLAYOUT_H
