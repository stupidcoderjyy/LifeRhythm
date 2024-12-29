//
// Created by stupid_coder_jyy on 2024/3/12.
//

#include "JbTextButton.h"
#include "Styles.h"
#include "NBT.h"
#include "FocusManager.h"
#include <QHBoxLayout>

USING_LR

JbTextButton::JbTextButton(QWidget *parent): FocusContainer(parent), type(Normal), running(false) {
    setObjectName("tb");
    content = new Label(this);
    content->setObjectName("tbc");
    content->setAlignment(Qt::AlignCenter);
    auto* layout = new QHBoxLayout(this);
    setLayout(layout);
    layout->addWidget(content);
    setFont(FontBuilder(Styles::FONT_TYPE_MAIN, Styles::FONT_MAIN).setSmallSize());
}

void JbTextButton::setButtonType(State t) {
    type = t;
    updateBase();
}

void JbTextButton::setButtonText(const QString &text) {
    content->setText(text);
    setFixedWidth(fontBuilder.getTextWidth(text) + 60);
    updateBase();
}

void JbTextButton::setFont(const FontBuilder &fb) {
    fontBuilder = fb;
    setFixedWidth(fontBuilder.getTextWidth(content->text()) + 60);
    setFixedHeight(fontBuilder.getTextHeight() + 10);
    updateBase();
}

void JbTextButton::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    auto btnType = widgetTag->getString("buttonType");
    State t = Normal;
    if (btnType == "Highlighted") {
        t = Highlighted;
    } else if (btnType == "Disabled") {
        t = Disabled;
    }
    handlers << [t](QWidget* target) {
        static_cast<JbTextButton *>(target)->setButtonType(t);
    };
    if (widgetTag->contains("text", Data::STRING)) {
        auto text = widgetTag->getString("text");
        handlers << [text](QWidget* target) {
            static_cast<JbTextButton*>(target)->setButtonText(text);
        };
    }
}

QString JbTextButton::getNormalQss() {
    layout()->setContentsMargins(0,0,0,0);
    switch (type) {
        case Highlighted:
            return qss_this("");
        case Normal:
            return qss_this(bg(Styles::CLEAR->rgbHex) + bd("1px", "solid", Styles::GRAY_2->rgbHex) + brad("3px"));
        default:
            return qss_this(bg(Styles::CLEAR->rgbHex) + bd("1px", "solid", Styles::GRAY_1->rgbHex) + brad("3px"));
    }
}

QString JbTextButton::getFocusedQss() {
    layout()->setContentsMargins(3,3,3,3);
    return FocusContainer::getFocusedQss();
}

void JbTextButton::mouseReleaseEvent(QMouseEvent *event) {
    if (type != Disabled) {
        emit sigActivated();
    }
}

void JbTextButton::mousePressEvent(QMouseEvent *event) {
    if (type != Disabled) {
        FocusContainer::mousePressEvent(event);
    }
}

void JbTextButton::resizeEvent(QResizeEvent *event) {
    FocusContainer::resizeEvent(event);
    if (!running) {
        running = true;
        updateBase();
    }
}

void JbTextButton::updateBase() {
    if (!running) {
        return;
    }
    QPalette p = content->palette();
    switch (type) {
        case Highlighted: {
            content->setStyleSheet(qss_object("tbc", bg(Styles::BLUE_0->rgbHex) + brad("2px") + clr(Styles::GRAY_TEXT_1->rgbHex)));
            auto fb = fontBuilder;
            content->setFont(fb.setBoldWeight().get());
            p.setColor(QPalette::WindowText, Styles::GRAY_TEXT_1->color);
            break;
        }
        case Normal: {
            content->setStyleSheet(qss_object("tbc", bg(Styles::CLEAR->rgbHex) + clr(Styles::GRAY_TEXT_1->rgbHex)));
            content->setFont(fontBuilder.get());
            break;
        }
        case Disabled: {
            content->setStyleSheet(qss_object("tbc", bg(Styles::CLEAR->rgbHex) + clr(Styles::GRAY_3->rgbHex)));
            content->setFont(fontBuilder.get());
            break;
        }
    }
    content->setPalette(p);
}
