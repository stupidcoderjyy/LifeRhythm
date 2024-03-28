//
// Created by stupid_coder_jyy on 2024/3/12.
//

#include "TextButton.h"
#include "Styles.h"
#include "NBT.h"
#include "WidgetUtil.h"
#include "FocusManager.h"
#include <QHBoxLayout>

TextButtonContent::TextButtonContent(QWidget *parent): TextLabel(parent) {
    setObjectName("tbc");
}

void TextButtonContent::mousePressEvent(QMouseEvent *ev) {
    ev->ignore();
}

void TextButtonContent::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

TextButton::TextButton(QWidget *parent): FocusContainer(parent), type(Normal) {
    content = new TextButtonContent(this);
    content->setAlignment(Qt::AlignCenter);
    auto* layout = new QHBoxLayout(this);
    setLayout(layout);
    layout->addWidget(content);
    setFixedHeight(30);
}

void TextButton::setButtonType(Type t) {
    type = t;
    QPalette p = content->palette();
    switch (type) {
        case Highlighted:
            content->setStyleSheet(qss_target("tbc", bg(Styles::BLUE_0->rgbHex) + brad("2px")));
            p.setColor(QPalette::WindowText, Styles::GRAY_TEXT_1->color);
            break;
        case Normal:
            content->setStyleSheet(qss_target("tbc", bg(Styles::CLEAR->rgbHex)));
            p.setColor(QPalette::WindowText, Styles::GRAY_TEXT_0->color);
            break;
        case Disabled:
            content->setStyleSheet(qss_target("tbc", bg(Styles::CLEAR->rgbHex)));
            p.setColor(QPalette::WindowText, Styles::GRAY_3->color);
            break;
    }
    content->setPalette(p);
    setStyleSheet(hasFocus() ? getFocusedQss() : getNormalQss());
}

void TextButton::setButtonText(const QString &text) {
    content->setText(text);
    QFontMetrics fm(content->font());
    setFixedWidth(fm.horizontalAdvance(text) + 80);
}

void TextButton::onPostParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    auto btnType = widgetTag->getString("buttonType");
    Type t = Normal;
    if (btnType == "Highlighted") {
        t = Highlighted;
    } else if (btnType == "Disabled") {
        t = Disabled;
    }
    handlers << [t](QWidget* target) {
        static_cast<TextButton *>(target)->setButtonType(t);
    };
    if (widgetTag->contains("text", Data::STRING)) {
        auto text = widgetTag->getString("text");
        handlers << [text](QWidget* target) {
            static_cast<TextButton*>(target)->setButtonText(text);
        };
    }
}

QString TextButton::getNormalQss() {
    layout()->setContentsMargins(0,0,0,0);
    switch (type) {
        case Highlighted:
            return qss_this("");
        case Normal:
            return qss_this(bg(Styles::CLEAR->rgbHex) + bd("1px", "solid", Styles::GRAY_2->rgbHex) + brad("3px"));
        case Disabled:
            return qss_this(bg(Styles::CLEAR->rgbHex) + bd("1px", "solid", Styles::GRAY_1->rgbHex) + brad("3px"));
    }
    return "";
}

void TextButton::mouseReleaseEvent(QMouseEvent *event) {
    if (type != Disabled) {
        emit sigActivated();
    }
}

void TextButton::mousePressEvent(QMouseEvent *event) {
    if (type != Disabled) {
        FocusContainer::mousePressEvent(event);
    }
}

QString TextButton::getFocusedQss() {
    layout()->setContentsMargins(3,3,3,3);
    return FocusContainer::getFocusedQss();
}