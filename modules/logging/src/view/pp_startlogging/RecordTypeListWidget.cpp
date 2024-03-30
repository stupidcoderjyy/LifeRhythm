//
// Created by stupid_coder_jyy on 2024/3/4.
//

#include "RecordTypeListWidget.h"
#include "WidgetFactory.h"
#include "RcManagers.h"
#include "RecordType.h"

RecordTypeItem::RecordTypeItem(QWidget *parent): SelectableListItem(parent), labelIcon(), labelName() {
}

void RecordTypeItem::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<RecordTypeItem*>(target)->init();
    };
}

void RecordTypeItem::syncDataToWidget() {
    SelectableListItem::syncDataToWidget();
    setState(selected);
    if (!wData) {
        setVisible(false);
        return;
    }
    auto* t = wData->cast<RecordType>();
    labelName->setText(t->name);
    auto* icon = ImageStorage::get(Identifier(t->iconPath));
    if (icon) {
        labelIcon->setPixmap(*icon);
    }
    setVisible(true);
}

void RecordTypeItem::init() {
    labelIcon = getPointer<ImgLabel>("icon");
    labelName = getPointer<TextLabel>("name");
}

SelectableListItem *RecordTypeListWidget::createRowItem() {
    auto* item = WidgetFactoryStorage::get("log:item_recordtype")->apply();
    return static_cast<SelectableListItem*>(item);
}

RecordTypeListWidget::RecordTypeListWidget(QWidget *parent): SelectableListWidget(parent) {
}
