//
// Created by stupid_coder_jyy on 2024/4/27.
//

#include "A1.h"
#include "DataA1.h"
#include "RcManagers.h"

USING_NAMESPACE(lr::log)

A1::A1(QWidget *parent): TreeWidget(parent) {
    setRowHeight(35);
}

ListItem *A1::createRowItem() {
    return WidgetFactoryStorage::get("log:item_a1")->applyAndCast<ListItem>();
}

ItemA1::ItemA1(QWidget *parent): TreeItem(parent), labelType(), labelTimeUsage(), active() {
}

void ItemA1::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* t) {
        static_cast<ItemA1*>(t)->init();
    };
}

void ItemA1::syncDataToWidget() {
    TreeItem::syncDataToWidget();
    TreeNode* node;
    DataA1* d = nullptr;
    if (wData) {
        node = wData->cast<TreeNode>();
        d = node->nodeData()->cast<DataA1>();
    }
    if (d) {
        active = d->isActive();
        layout()->setContentsMargins(10 + node->getDepth() * 20, 0, 0, 0);
        labelType->syncDataToWidget();
        labelName->setVisible(true);
        labelName->setText(d->getUsage()->getName());
        labelTimeUsage->setVisible(true);
        labelTimeUsage->setText("[" + usageText(d->getDuration()) + "]");
        labelTimeUsage->setStyleSheet(qss("color", active ? Styles::GREEN_2->rgbHex : Styles::GRAY_4->rgbHex));
    } else {
        labelTimeUsage->setVisible(false);
        labelName->setVisible(false);
    }
}

void ItemA1::setData(WidgetData *d) {
    StandardWidget::setData(d);
    UsageType* ut = nullptr;
    if (d) {
        if (auto* da = d->cast<TreeNode>()->nodeData()->cast<DataA1>()) {
            ut = da->getUsage()->getType();
        }
    }
    labelType->setData(ut);
}

void ItemA1::connectModelView() {
    TreeItem::connectModelView();
    dc << connect(wData, &WidgetData::sigDataChanged, this, &ItemA1::syncDataToWidget);
}

void ItemA1::init() {
    labelType = getPointer<UsageTypeLabel>("l1");
    labelName = getPointer<TextLabel>("l2");
    labelTimeUsage = getPointer<TextLabel>("l3");
}

QString ItemA1::usageText(int time) {
    int h = time / 60;
    int m = time % 60;
    if (h == 0) {
        return QString::number(m) + "分钟";
    }
    QString res = QString::number(h) + "小时";
    if (m != 0) {
        res += QString::number(m) + "分钟";
    }
    return res;
}

void ItemA1::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    if (active) {
        p.fillRect(rect(), Styles::GREEN_0->color);
        p.fillRect(0, 0, 5, 40, Styles::GREEN_1->color);
    } else {
        p.fillRect(rect(), Styles::BLACK->color);
    }
}
