//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "Button.h"

#include <NBT.h>

#include "Styles.h"
#include "WidgetUtil.h"
#include "QMouseEvent"
#include "FocusManager.h"

#define qssNormal bg(Styles::CLEAR->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px")
#define qssHovered bg(Styles::GRAY_1->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px")
#define qssPressed bg(Styles::GRAY_2->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px")
#define qssSelected bg(Styles::GRAY_2->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px")
#define qssSelectedDisabled bg(Styles::CLEAR->rgbHex) + bd("2px", "solid", Styles::GRAY_1->rgbHex) + brad("2px")

Button::Button(QWidget *parent, bool initInConstructor):
Label(parent, initInConstructor), running(), selected(), hasStyle(true), activatedOnPress(), hasImg(),
enabled(true), type(Click) {
    setAlignment(Qt::AlignCenter);
}

void Button::setButtonText(const QString &text) {
    hasImg = false;
    QFontMetrics metrics(font());
    setMinimumSize(metrics.horizontalAdvance(text) + 10, metrics.height() + 10);
    setText(text);
}

void Button::setButtonImg(const QPixmap &p1, const QPixmap& p2) {
    hasImg = true;
    imgNormal = p1;
    imgDisabled = p2.isNull() ? p1 : p2;
    auto p = isEnabled() ? imgNormal : imgDisabled;
    setFixedSize(p.size());
    setPixmap(p);
}

void Button::setButtonEnabled(bool e) {
    if (enabled == e) {
        return;
    }
    enabled = e;
    if (!hasStyle) {
        return;
    }
    if (enabled) {
        if (hasImg) {
            setPixmap(imgNormal);
            setFixedSize(imgNormal.size());
            if (type == Select && selected) {
                setStyleSheet(qssSelected);
            }
        } else {
            auto p = palette();
            p.setColor(QPalette::WindowText, Styles::GRAY_TEXT_0->color);
            p.setColor(QPalette::Text, Styles::GRAY_TEXT_0->color);
            setPalette(p);
        }
    } else {
        if (type == Select && selected) {
            setStyleSheet(qssSelectedDisabled);
        } else {
            setStyleSheet(qssNormal);
        }
        if (hasImg) {
            setPixmap(imgDisabled);
            setFixedSize(imgDisabled.size());
        } else {
            auto p = palette();
            p.setColor(QPalette::WindowText, Styles::GRAY_4->color);
            p.setColor(QPalette::Text, Styles::GRAY_4->color);
            setPalette(p);
        }
    }
}

void Button::setButtonStyleEnabled(bool e) {
    if (enabled != e) {
        enabled = e;
        if (!e) {
            setStyleSheet(qssNormal);
        }
    }
}

void Button::setSelected(bool s) {
    if (!enabled || selected == s || type == Click) {
        return;
    }
    selected = s;
    emit sigSelected(s);
    if (!hasStyle) {
        return;
    }
    if (type == Select) {
        if (s) {
            setStyleSheet(qssSelected);
        } else if (isMouseHovered(this)) {
            setStyleSheet(qssHovered);
        } else {
            setStyleSheet(qssNormal);
        }
    }
}

void Button::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    Label::onPostParsing(handlers, widgetTag);
    if (widgetTag->contains("mode", Data::STRING)) {
        Mode mode = Click;
        auto modeStr = widgetTag->getString("mode");
        if (modeStr == "Select") {
            mode = Select;
        } else if (modeStr == "SelectClick") {
            mode = SelectClick;
        }
        handlers << [mode](QWidget* w) {
            static_cast<Button*>(w)->setButtonMode(mode);
        };
    }
    if (widgetTag->contains("styleEnabled", Data::BOOL)) {
        bool styleEnabled = widgetTag->getBool("styleEnabled");
        handlers << [styleEnabled](QWidget* w) {
            static_cast<Button*>(w)->setButtonStyleEnabled(styleEnabled);
        };
    }
    if (widgetTag->contains("activeOnPress", Data::BOOL)) {
        bool activeOnPress = widgetTag->getBool("activeOnPress");
        handlers << [activeOnPress](QWidget* w) {
            static_cast<Button*>(w)->setActivateOnPress(activeOnPress);
        };
    }
}

void Button::enterEvent(QEvent *event) {
    if (!enabled) {
        return;
    }
    if (hasStyle && (type != Select || !selected)) {
        setStyleSheet(qssHovered);
    }
}

void Button::mouseReleaseEvent(QMouseEvent *ev) {
    if (!enabled) {
        return;
    }
    if (!activatedOnPress) {
        handleButtonActivate(ev);
    }
}

void Button::mousePressEvent(QMouseEvent *ev) {
    if (!enabled) {
        return;
    }
    if (hasStyle) {
        setStyleSheet(qssPressed);
    }
    FocusManager::mark(this);
    if (activatedOnPress) {
        handleButtonActivate(ev);
    }
}

void Button::leaveEvent(QEvent *event) {
    if (!enabled) {
        return;
    }
    if (hasStyle && (type != Select || !selected)) {
        setStyleSheet(qssNormal);
    }
}

void Button::handleButtonActivate(QMouseEvent *ev) {
    if (!isHovered(this, ev)) {
        return;
    }
    if (hasStyle && (type == Click || type == SelectClick)) {
        setStyleSheet(qssHovered);
    }
    if (type == Click) {
        emit sigActivated();
    } else {
        setSelected(!selected);
    }
}
