//
// Created by stupid_coder_jyy on 2024/3/20.
//

#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H

#include <QGridLayout>
#include "Namespaces.h"
#include "Plugin.h"

BEGIN_LR

class CORE_API GridLayout : public QGridLayout {
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

END_LR

#endif //GRIDLAYOUT_H
