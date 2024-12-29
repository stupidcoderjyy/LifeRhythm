//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "Label.h"
#include "Data.h"
#include "NBT.h"
#include <QMouseEvent>
#include "WidgetFactory.h"
#include "ImageStorage.h"

USING_LR

Label::Label(QWidget *parent, bool initInConstructor): QLabel(parent), StandardWidget(initInConstructor) {
    setAlignment(Qt::AlignCenter);
    auto p = palette();
    p.setColor(QPalette::WindowText, Styles::GRAY_TEXT_1->color);
    p.setColor(QPalette::Text, Styles::GRAY_TEXT_1->color);
    setPalette(p);
    setFont(Styles::FONT_SMALL);
}

void Label::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    WidgetFactory::parseTextWidget<QLabel>(handlers, widgetTag);
    if (widgetTag->contains("img", Data::STRING)) {
        auto loc = Identifier(widgetTag->getString("img"));
        QPixmap img{};
        if (ImageStorage::exists(loc)) {
            img = *ImageStorage::get(loc);
        }
        if (!img.isNull() && widgetTag->contains("scale", Data::ARR)) {
            QSize scale = WidgetFactory::parseSize(widgetTag->get("scale")->asArray());
            img = img.scaled(scale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
        handlers << [img](QWidget* target) {
            static_cast<Label*>(target)->setPixmap(img);
        };
    }
}

void Label::onStateRespondersParsing(Handlers &responders, NBT *stateTag) {
    onPostParsing(responders, stateTag);
}

QSize Label::baseTextSize(const QString &text, const QFont &font) {
    QFontMetrics metrics(font);
    return QSize(metrics.horizontalAdvance(text) + 10, metrics.height() + 10);
}

void Label::mouseReleaseEvent(QMouseEvent *ev) {
    ev->ignore();
}

void Label::mousePressEvent(QMouseEvent *ev) {
    ev->ignore();
}