//
// Created by stupid_coder_jyy on 2024/3/8.
//

#ifndef LIFERHYTHM_PERIODTYPETREEWIDGET_H
#define LIFERHYTHM_PERIODTYPETREEWIDGET_H

#include "SelectableTreeWidget.h"
#include "PeriodType.h"

class PeriodTypeIcon : public Widget {
    friend class PeriodTypeTreeItem;
public:
    explicit PeriodTypeIcon(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

class ArrowButton;
class TextLabel;

class PeriodTypeTreeItem : public SelectableTreeItem {
private:
    ArrowButton* buttonFoldItem;
    PeriodTypeIcon* icon;
    TextLabel* labelName;
public:
    explicit PeriodTypeTreeItem(QWidget* parent = nullptr);
    void syncDataToWidget() override;
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
private:
    void init();
};

class PeriodTypeTreeWidget : public SelectableTreeWidget {
public:
    explicit PeriodTypeTreeWidget(QWidget* parent = nullptr);
protected:
    SelectableTreeItem *createRowItem() override;
};


#endif //LIFERHYTHM_PERIODTYPETREEWIDGET_H
