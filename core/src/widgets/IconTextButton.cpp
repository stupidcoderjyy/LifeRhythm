//
// Created by stupid_coder_jyy on 2024/3/30.
//

#include "IconTextButton.h"
#include "RcManagers.h"
#include "WidgetUtil.h"
#include "ImgLabel.h"
#include "TextLabel.h"

IconTextButton::IconTextButton(QWidget *parent): FocusContainer(parent),
        hasInit(), icon(), name() {
}

void IconTextButton::set(const Identifier &iconLoc, const QString &s) {
    if (!hasInit) {
        init();
    }
    auto* img = ImageStorage::get(iconLoc);
    if (img) {
        icon->setPixmap(*img);
    }
    setText(s);
}

void IconTextButton::onPostParsing(StandardWidget::Handlers &handlers, NBT *nbt) {
    handlers << [](QWidget* target) {
        static_cast<IconTextButton*>(target)->init();
    };
    if (nbt->contains("icon", Data::STRING)) {
        auto loc = Identifier(nbt->getString("icon"));
        handlers << [loc](QWidget* target) {
            auto* img = ImageStorage::get(loc);
            if (img) {
                static_cast<IconTextButton*>(target)->icon->setPixmap(*img);
            }
        };
    }
    if (nbt->contains("text", Data::STRING)) {
        auto text = nbt->getString("text");
        handlers << [text](QWidget* target) {
            static_cast<IconTextButton*>(target)->setText(text);
        };
    }
}

void IconTextButton::mouseReleaseEvent(QMouseEvent *event) {
    if (isMouseHovered(this)) {
        emit sigActivated();
    }
}

void IconTextButton::resizeEvent(QResizeEvent *event) {
    if (!hasInit) {
        init();
    }
    FocusContainer::resizeEvent(event);
}

void IconTextButton::init() {
    WidgetFactoryStorage::get("lr:widget_icontextbutton")->apply(nullptr, this);
    icon = getPointer<ImgLabel>("icon");
    name = getPointer<TextLabel>("name");
    hasInit = true;
    connect(icon, &ImgLabel::sigActivated, this, &IconTextButton::sigActivated);
}

void IconTextButton::setText(const QString &text) {
    name->setText(text);
    QFontMetrics fm(name->font());
    name->setFixedWidth(fm.horizontalAdvance(text) + 20);
}
