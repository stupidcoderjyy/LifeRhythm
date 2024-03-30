//
// Created by stupid_coder_jyy on 2024/3/25.
//

#include "PeriodTypeColorBox.h"
#include "RcManagers.h"
#include "ColorSelector.h"
#include <QHBoxLayout>

PeriodTypeColorBox::PeriodTypeColorBox(QWidget *parent): AbstractOptionsBox(parent) {
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(3,3,3,3);
    layout->setSpacing(0);
    setLayout(layout);
    iconData = new Color;
    iconData->color.setHsvF(0, 0, 0.7);
    icon = new ColorIcon(this);
    icon->setBorderWidth(0);
    icon->setFixedSize(27,27);
    icon->setData(iconData);
    layout->addWidget(icon);
}

PeriodTypeColorBox::~PeriodTypeColorBox() {
    delete iconData;
}

void PeriodTypeColorBox::initMenu(OptionsMenu *menu) {
    menu->setFixedWidth(300);
    hueSelector = WidgetFactoryStorage::get("lr:widget_hueselector")->applyAndCast<HueSelector>();
    connect(hueSelector, &HueSelector::sigColorSelected, icon, [this](const QColor& c){
        iconData->color.setHsvF(c.hueF(), 0.4, 0.7);
        icon->update();
    });
    auto* l = new QHBoxLayout(menu);
    l->setContentsMargins(2,2,2,2);
    l->addWidget(hueSelector);
    menu->setLayout(l);
}

void PeriodTypeColorBox::mousePressEvent(QMouseEvent *event) {
    AbstractOptionsBox::mousePressEvent(event);
    if (!menuOpen && !pressLock) {
        clickBox(true);
    }
}