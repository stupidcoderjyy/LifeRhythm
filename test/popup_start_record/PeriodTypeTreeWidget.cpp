//
// Created by stupid_coder_jyy on 2024/3/8.
//

#include "PeriodTypeTreeWidget.h"
#include "ArrowButton.h"
#include "TextLabel.h"
#include "RcManagers.h"

PeriodTypeIcon::PeriodTypeIcon(QWidget *parent): Widget(parent) {
}

void PeriodTypeIcon::paintEvent(QPaintEvent *event) {
    if (!wData) {
        return;
    }
    int fw = 2, dfw = fw << 1, d = 5;
    QPainter painter(this);
    auto* type = wData->cast<PeriodType>();
    if (type->getChildren().empty()) {
        painter.fillRect(rect(), QColor(Styles::GRAY_TEXT_0));
        painter.fillRect(fw, fw, width() - dfw, height() - dfw, type->color);
    } else {
        int dw = width() - d, dh = height() - d;
        painter.fillRect(0, 0, dw, dh, QColor(Styles::GRAY_TEXT_0));
        painter.fillRect(fw, fw, dw - dfw, dh - dfw, type->color);
        painter.fillRect(d, d, dw, dh, QColor(Styles::GRAY_TEXT_0));
        painter.fillRect(d + fw, d + fw, dw - dfw, dh - dfw, type->color);
    }
}

PeriodTypeTreeItem::PeriodTypeTreeItem(QWidget *parent): SelectableTreeItem(parent) {
}

void PeriodTypeTreeItem::syncDataToWidget() {
    SelectableTreeItem::syncDataToWidget();
    setState(selected);
    icon->setData(wData);
    if (wData) {
        auto* node = wData->cast<PeriodType>();
        labelName->setText(node->name);
        layout()->setContentsMargins(node->depth * 20, 0, 0, 0);
        buttonFoldItem->setBaseType(node->children.empty() ? ArrowButton::None : ArrowButton::Right);
        buttonFoldItem->setPressed(!node->folded);
    }
    buttonFoldItem->setVisible(wData);
    icon->setVisible(wData);
    labelName->setVisible(wData);
}

void PeriodTypeTreeItem::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<PeriodTypeTreeItem*>(target)->init();
    };
}

void PeriodTypeTreeItem::init() {
    buttonFoldItem = getPointer<ArrowButton>("arrowButton");
    buttonFoldItem->setVisible(false);
    connect(buttonFoldItem, &ArrowButton::sigActivated, this, [this](bool pressed){
        emit sigItemFold(dataIdx, !pressed);
    });
    icon = getPointer<PeriodTypeIcon>("icon");
    icon->setVisible(false);
    labelName = getPointer<TextLabel>("label");
    labelName->setVisible(false);
}

PeriodTypeTreeWidget::PeriodTypeTreeWidget(QWidget *parent): SelectableTreeWidget(parent) {
    setRowHeight(30);
}

SelectableTreeItem *PeriodTypeTreeWidget::createRowItem() {
    return WidgetFactoryStorage::get("log:item_period_type")->applyAndCast<PeriodTypeTreeItem>();
}
