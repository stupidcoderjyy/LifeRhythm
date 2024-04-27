//
// Created by stupid_coder_jyy on 2024/4/27.
//

#ifndef LIFERHYTHM_A1_H
#define LIFERHYTHM_A1_H

#include "Namespaces.h"
#include "TreeWidget.h"
#include "UsageTypeLabel.h"
#include "TextLabel.h"

BEGIN_NAMESPACE(lr::log)

class A1 : public TreeWidget {
public:
    explicit A1(QWidget* parent = nullptr);
protected:
    ListItem *createRowItem() override;
};

class ItemA1 : public TreeItem {
private:
    bool active;
    UsageTypeLabel* labelType;
    TextLabel* labelName;
    TextLabel* labelTimeUsage;
public:
    explicit ItemA1(QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void syncDataToWidget() override;
    void setData(WidgetData *d) override;
protected:
    void connectModelView() override;
    void paintEvent(QPaintEvent *event) override;
private:
    void init();
    static QString usageText(int time);
};

END_NAMESPACE

#endif //LIFERHYTHM_A1_H
