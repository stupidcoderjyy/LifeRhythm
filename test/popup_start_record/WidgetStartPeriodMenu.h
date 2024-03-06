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
    SelectableListData* modelRecordTypes;
public:
    static void mainInit();
    explicit WidgetStartPeriodMenu(QWidget* parent = nullptr);
    void setRecordTypeData(SelectableListData *modelRecordTypes, QVector<QWidget*>* configPages);
};

#endif //LIFERHYTHM_WIDGETSTARTPERIODMENU_H
