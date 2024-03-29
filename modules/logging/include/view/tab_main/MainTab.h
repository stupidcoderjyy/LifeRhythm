//
// Created by stupid_coder_jyy on 2024/3/2.
//

#ifndef LIFERHYTHM_MAINTAB_H
#define LIFERHYTHM_MAINTAB_H

#include "TimeBar.h"
#include "TabBar.h"
#include "SelectableListData.h"

class PeriodDataLoader;

class MainTab : public lr::TabWidget {
    Q_OBJECT
private:
    TimeBar* bar;
    PeriodDataLoader* loader;
    SelectableListData* modelRecordTypeList;
    QVector<QWidget*> startRecordMenuPages;
public:
    static void mainInit();
    explicit MainTab(QWidget* parent = nullptr);
    void addStartPeriodType(QString icon, QString typeName, QWidget* menuPage);
protected:
    void onTabCreated() override;
};

#endif //LIFERHYTHM_MAINTAB_H
