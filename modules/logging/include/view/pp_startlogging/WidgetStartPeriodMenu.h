//
// Created by stupid_coder_jyy on 2024/3/4.
//

#ifndef LIFERHYTHM_WIDGETSTARTPERIODMENU_H
#define LIFERHYTHM_WIDGETSTARTPERIODMENU_H

#include "Widget.h"
#include "RecordTypeListWidget.h"

class WidgetStartPeriodMenu : public Widget {
    Q_OBJECT
private:
    QVector<QWidget*>* configPages;
    QLayout* layoutPage;
    RecordTypeListWidget* recordTypeListWidget;
    ListData* modelRecordTypes;
public:
    explicit WidgetStartPeriodMenu(QWidget* parent = nullptr);
    void setRecordTypeData(ListData *modelRecordTypes, QVector<QWidget*>* configPages);
};

#endif //LIFERHYTHM_WIDGETSTARTPERIODMENU_H
