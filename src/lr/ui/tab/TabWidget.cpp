//
// Created by stupid_coder_jyy on 2024/2/9.
//

#include "lr/ui/tab/TabWidget.h"
#include "StdImgLabel.h"
#include "StdTextLabel.h"
#include "ImageStorage.h"

TabWidget::TabWidget(QWidget *parent) : StdWidget(parent) {
}

void TabWidget::onFinishedParsing(Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        auto* tab = static_cast<TabWidget*>(target);
        tab->iconLabel = tab->getPointer<StdImgLabel>("icon");
        tab->titleLabel = tab->getPointer<StdTextLabel>("title");
        tab->closeButton = tab->getPointer<CloseButton>("closeButton");
        connect(tab->closeButton, &CloseButton::sigActivated, tab, &TabWidget::sigTabClosed);
    };
}

void TabWidget::mousePressEvent(QMouseEvent *event) {
    emit sigTabActivated();
    selected = true;
    setState(3);
}

void TabWidget::enterEvent(QEvent *event) {
    setState(selected ? 3 : 1);
}

void TabWidget::leaveEvent(QEvent *event) {
    setState(selected ? 2 : 0);
}

void TabWidget::setIcon(const Identifier &id) {
    QPixmap* img = ImageStorage::getInstance()->get(id);
    if (img) {
        iconLabel->setPixmap(*img);
    }
}

void TabWidget::setTitle(const QString &title) {
    titleLabel->setText(title);
}

CloseButton::CloseButton(QWidget *parent) : StdImgLabel(parent) {
}

void CloseButton::enterEvent(QEvent *event) {
    setState(1);
}

void CloseButton::leaveEvent(QEvent *event) {
    setState(0);
}

void CloseButton::mouseReleaseEvent(QMouseEvent *ev) {
    emit sigActivated();
}
