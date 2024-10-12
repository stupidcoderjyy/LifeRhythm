//
// Created by stupid_coder_jyy on 2024/3/4.
//

#include "RecordTypeListWidget.h"
#include "WidgetFactory.h"
#include "RcManagers.h"
#include "RecordType.h"

RecordTypeItem::RecordTypeItem(QWidget *parent): ListItem(parent), labelIcon(), labelName() {
}

void RecordTypeItem::onFinishedParsing(Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<RecordTypeItem*>(target)->init();
    };
}

void RecordTypeItem::syncDataToWidget() {
    ListItem::syncDataToWidget();
    setState(selected);
    if (!wData) {
        setVisible(false);
        return;
    }
    auto* t = wData->cast<RecordType>();
    labelName->setText(t->name);
    if (auto* icon = ImageStorage::get(Identifier(t->iconPath))) {
        labelIcon->setPixmap(*icon);
    }
    setVisible(true);
}

void RecordTypeItem::init() {
    labelIcon = getPointer<ImgLabel>("icon");
    labelName = getPointer<TextLabel>("name");
}

ListItem *RecordTypeListWidget::createRowItem() {
    auto* item = WidgetFactoryStorage::get("log:item_recordtype")->apply();
    return static_cast<ListItem*>(item);
}

RecordTypeListWidget::RecordTypeListWidget(QWidget *parent): ListWidget(parent) {
}
