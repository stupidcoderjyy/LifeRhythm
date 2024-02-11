//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "StdImgLabel.h"
#include "Data.h"
#include "NBT.h"
#include "ImageStorage.h"
#include "WidgetFactoryParsers.h"
#include "WidgetUtil.h"

StdImgLabel::StdImgLabel(QWidget *parent):QLabel(parent),StandardWidget() {
}

void StdImgLabel::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    QPixmap img{};
    if (widgetTag->contains("img", Data::STRING)) {
        Identifier loc = Identifier(widgetTag->get("img")->asString()->get());
        img = *ImageStorage::getInstance()->get(loc);
    }
    if (!img.isNull() && widgetTag->contains("scale", Data::ARR)) {
        QSize scale = WidgetFactoryParsers::parseSize(widgetTag->get("scale")->asArray());
        img = img.scaled(scale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    Qt::Alignment align = Qt::AlignCenter;
    if (widgetTag->contains("align", Data::STRING)) {
        align = WidgetFactoryParsers::parseAlign(widgetTag->getString("align"));
    }
    handlers << [img, align](QWidget* target) {
        auto* label = static_cast<StdImgLabel*>(target);
        if (!img.isNull()) {
            label->setMinimumSize(img.width(), img.height());
            label->setPixmap(img);
        } else {
            label->setPixmap({});
        }
        label->setAlignment(align);
    };
}

void StdImgLabel::onStateRespondersParsing(Handlers &responders, NBT *stateTag) {
    onPostParsing(responders, stateTag);
}

void StdImgLabel::mouseReleaseEvent(QMouseEvent *ev) {
    if (isHovered(this, ev)) {
        emit sigActivated();
    }
}
