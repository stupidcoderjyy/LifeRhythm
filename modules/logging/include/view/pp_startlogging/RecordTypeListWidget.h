//
// Created by stupid_coder_jyy on 2024/3/4.
//

#ifndef LIFERHYTHM_RECORDTYPELISTWIDGET_H
#define LIFERHYTHM_RECORDTYPELISTWIDGET_H

#include "ImgLabel.h"
#include "ListWidget.h"
#include "TextLabel.h"

class RecordTypeItem : public ListItem {
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

class RecordTypeListWidget : public ListWidget {
protected:
    ListItem *createRowItem() override;
public:
    explicit RecordTypeListWidget(QWidget* parent = nullptr);
};


#endif //LIFERHYTHM_RECORDTYPELISTWIDGET_H
