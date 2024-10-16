//
// Created by stupid_coder_jyy on 2024/3/8.
//

#include "UsageTypeTreeWidget.h"
#include <QLayout>
#include "ArrowButton.h"
#include "TextLabel.h"
#include "RcManagers.h"
#include "UsageType.h"
#include "UsageTypeIcon.h"

USING_NAMESPACE(lr::log)

UsageTypeTreeItem::UsageTypeTreeItem(QWidget *parent): TreeItem(parent), buttonFoldItem(), icon(), labelName() {
}

void UsageTypeTreeItem::syncDataToWidget() {
    TreeItem::syncDataToWidget();
    setState(selected);
    icon->setData(wData);
    if (wData) {
        auto* node = wData->cast<TreeNode>();
        auto* type = node->nodeData()->cast<UsageType>();
        labelName->setText(type->getName());
        layout()->setContentsMargins(node->getDepth() * 20, 0, 0, 0);
        buttonFoldItem->setBaseType(node->getChildren().empty() ? ArrowButton::None : ArrowButton::Right);
        buttonFoldItem->setPressed(!node->isFolded());
    }
    buttonFoldItem->setVisible(wData);
    icon->setVisible(wData);
    labelName->setVisible(wData);
}

void UsageTypeTreeItem::onFinishedParsing(Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<UsageTypeTreeItem*>(target)->init();
    };
}

void UsageTypeTreeItem::init() {
    buttonFoldItem = getPointer<ArrowButton>("arrowButton");
    buttonFoldItem->setVisible(false);
    connect(buttonFoldItem, &ArrowButton::sigActivated, this, [this](bool pressed){
        emit sigItemFold(dataIdx, !pressed);
    });
    icon = getPointer<UsageTypeIcon>("icon");
    icon->setVisible(false);
    labelName = getPointer<TextLabel>("label");
    labelName->setVisible(false);
}

UsageTypeTreeWidget::UsageTypeTreeWidget(QWidget *parent): TreeWidget(parent) {
    setRowHeight(30);
}

TreeItem *UsageTypeTreeWidget::createRowItem() {
    return WidgetFactoryStorage::get("log:item_periodtype")->applyAndCast<UsageTypeTreeItem>();
}
