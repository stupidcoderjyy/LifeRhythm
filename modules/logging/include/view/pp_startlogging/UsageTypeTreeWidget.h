//
// Created by stupid_coder_jyy on 2024/3/8.
//

#ifndef LIFERHYTHM_USAGETYPETREEWIDGET_H
#define LIFERHYTHM_USAGETYPETREEWIDGET_H

#include "Namespaces.h"
#include "NBT.h"
#include "TreeWidget.h"

class ArrowButton;
class TextLabel;

BEGIN_NAMESPACE(lr::log)

class UsageTypeIcon;

class UsageTypeTreeItem : public TreeItem {
private:
    ArrowButton* buttonFoldItem;
    UsageTypeIcon* icon;
    TextLabel* labelName;
public:
    explicit UsageTypeTreeItem(QWidget* parent = nullptr);
    void syncDataToWidget() override;
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
private:
    void init();
};

class UsageTypeTreeWidget : public TreeWidget {
public:
    explicit UsageTypeTreeWidget(QWidget* parent = nullptr);
protected:
    TreeItem *createRowItem() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_USAGETYPETREEWIDGET_H
