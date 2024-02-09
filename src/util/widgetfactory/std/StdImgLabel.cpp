//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "StdImgLabel.h"
#include "Data.h"
#include "NBT.h"
#include "ImageStorage.h"

StdImgLabel::StdImgLabel(QWidget *parent):QLabel(parent),StandardWidget() {
}

void StdImgLabel::postParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    QPixmap* img = nullptr;
    if (widgetTag->contains("img", Data::STRING)) {
        Identifier loc = Identifier(widgetTag->get("img")->asString()->get());
        img = ImageStorage::getInstance()->get(loc);
    }
    handlers << [img](QWidget* target) {
        auto* label = static_cast<StdImgLabel*>(target);
        if (img) {
            label->setFixedSize(img->width() + 5, img->height() + 5);
            label->setPixmap(*img);
        } else {
            label->setPixmap({});
        }
        label->setAlignment(Qt::AlignCenter);
    };
}

void StdImgLabel::mouseReleaseEvent(QMouseEvent *ev) {
    emit sigActivated();
}
