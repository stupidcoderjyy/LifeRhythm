//
// Created by stupid_coder_jyy on 2024/3/4.
//

#ifndef LIFERHYTHM_RECORDTYPELISTWIDGET_H
#define LIFERHYTHM_RECORDTYPELISTWIDGET_H

#include "SelectableListWidget.h"
#include "ImgLabel.h"
#include "TextLabel.h"

class RecordTypeItem : public SelectableListItem {
private:
    ImgLabel* labelIcon;
    TextLabel* labelName;
public:
    explicit RecordTypeItem(QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void syncDataToWidget() override;
private:
    void init();
};

class RecordTypeListWidget : public SelectableListWidget {
protected:
    SelectableListItem *createRowItem() override;
public:
    explicit RecordTypeListWidget(QWidget* parent = nullptr);
};


#endif //LIFERHYTHM_RECORDTYPELISTWIDGET_H
