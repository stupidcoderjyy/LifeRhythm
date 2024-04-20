//
// Created by stupid_coder_jyy on 2024/4/20.
//

#include "Separator.h"
#include "NBT.h"

Separator::Separator(QWidget *parent): Widget(parent), color(Styles::GRAY_2->color) {
}

void Separator::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.fillRect(rect(), color);
}

void Separator::onPostParsing(StandardWidget::Handlers &handlers, NBT *nbt) {
    if (nbt->getBool("isHorizontal")) {
        handlers << [](QWidget* target) {
            target->setFixedHeight(1);
            target->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        };
    } else {
        handlers << [](QWidget* target) {
            target->setFixedWidth(1);
            target->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        };
    }
    if (nbt->contains("color", Data::STRING)) {
        QColor c = Styles::parseColor(nbt->getString("color"));
        handlers << [c](QWidget* target) {
            static_cast<Separator*>(target)->color = c;
        };
    }
}