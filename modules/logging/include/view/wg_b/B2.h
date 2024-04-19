//
// Created by stupid_coder_jyy on 2024/4/18.
//

#ifndef LIFERHYTHM_B2_H
#define LIFERHYTHM_B2_H

#include "ListWidget.h"
#include "Namespaces.h"

BEGIN_NAMESPACE(lr::log)

class TimeUsageItem : public ListItem {
private:
    bool shouldDraw;
    QColor barColor;
    int usagePerc;
    QString usageName;
    QString usage;
public:
    explicit TimeUsageItem(QWidget* parent = nullptr);
    void syncDataToWidget() override;
protected:
    void paintEvent(QPaintEvent *event) override;
};

class B2 : public ListWidget {
public:
    explicit B2(QWidget* parent = nullptr);
protected:
    ListItem *createRowItem() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_B2_H
