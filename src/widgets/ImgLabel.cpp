//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "ImgLabel.h"
#include "Data.h"
#include "NBT.h"
#include "RcManagers.h"
#include "WidgetUtil.h"

ImgLabel::ImgLabel(QWidget *parent): QLabel(parent), StandardWidget() {
    setAlignment(Qt::AlignCenter);
}

void ImgLabel::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    if (widgetTag->contains("img", Data::STRING)) {
        Identifier loc = Identifier(widgetTag->getString("img"));
        QPixmap img{};
        if (ImageStorage::exists(loc)) {
            img = *ImageStorage::get(loc);
        }
        if (!img.isNull() && widgetTag->contains("scale", Data::ARR)) {
            QSize scale = WidgetFactory::parseSize(widgetTag->get("scale")->asArray());
            img = img.scaled(scale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
        handlers << [img](QWidget* target) {
            auto* label = static_cast<ImgLabel*>(target);
            if (!img.isNull()) {
                label->setMinimumSize(img.width(), img.height());
                label->setPixmap(img);
            } else {
                label->setPixmap({});
            }
        };
    }
    if (widgetTag->contains("align", Data::STRING)) {
        Qt::Alignment align = WidgetFactory::parseAlign(widgetTag->getString("align"));
        handlers << [align](QWidget* target) {
            auto* label = static_cast<ImgLabel*>(target);
            label->setAlignment(align);
        };
    }
}

void ImgLabel::onStateRespondersParsing(Handlers &responders, NBT *stateTag) {
    onPostParsing(responders, stateTag);
}

void ImgLabel::mouseReleaseEvent(QMouseEvent *ev) {
    if (isHovered(this, ev)) {
        emit sigActivated();
    }
}
