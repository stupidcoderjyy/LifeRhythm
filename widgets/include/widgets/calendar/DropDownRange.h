//
// Created by JYY on 24-12-13.
//

#ifndef DROPDOWNRANGE_H
#define DROPDOWNRANGE_H
#include <Namespaces.h>
#include <WidgetData.h>
#include <Label.h>
#include <ListWidget.h>
#include <DropDown.h>
#include <Common.h>

BEGIN_NP(lr::calendar)

class DataRange : public WidgetData {
    friend class ItemRange;
private:
    int days;
public:
    explicit DataRange(int days);
};

class ItemRange : public ListItem {
private:
    Label* label;
public:
    explicit ItemRange(QWidget* parent = nullptr, bool iic = true);
    void initWidget() override;
    void syncDataToWidget() override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

class DropDownRange : public DropDown {
private:
    Label* label;
    ListWidget* list;
    ViewType viewType;
    ListData* optionsData;
public:
    explicit DropDownRange(QWidget* parent = nullptr, bool iic = true);
    void syncWidgetToData() override;
    void initWidget() override;
    ~DropDownRange() override;
};

END_NP

#endif //DROPDOWNRANGE_H
