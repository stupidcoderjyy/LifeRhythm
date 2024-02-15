//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "widgets/ImgLabel.h"
#include "Data.h"
#include "NBT.h"
#include "RcManagers.h"
#include "WidgetFactoryParsers.h"
#include "WidgetUtil.h"

ImgLabel::ImgLabel(QWidget *parent): QLabel(parent), StandardWidget() {
}

void ImgLabel::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    QPixmap img{};
    if (widgetTag->contains("img", Data::STRING)) {
        Identifier loc = Identifier(widgetTag->getString("img"));
        if (ImageStorage::exists(loc)) {
            img = *ImageStorage::get(loc);
        }
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
        auto* label = static_cast<ImgLabel*>(target);
        if (!img.isNull()) {
            label->setMinimumSize(img.width(), img.height());
            label->setPixmap(img);
        } else {
            label->setPixmap({});
        }
        label->setAlignment(align);
    };
}

void ImgLabel::onStateRespondersParsing(Handlers &responders, NBT *stateTag) {
    onPostParsing(responders, stateTag);
}

void ImgLabel::mouseReleaseEvent(QMouseEvent *ev) {
    if (isHovered(this, ev)) {
        emit sigActivated();
    }
}
