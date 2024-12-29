//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "Button.h"
#include <FocusManager.h>
#include <NBT.h>
#include "Styles.h"
#include "QMouseEvent"
#include <QDebug>
#include "Helpers.h"
#include "ImageStorage.h"
#include "WidgetFactory.h"

USING_LR

QString Button::QSS_NORMAL{};
QString Button::QSS_HOVERED{};
QString Button::QSS_PRESSED{};
QString Button::QSS_SELECTED{};
QString Button::QSS_SELECTED_DISABLED{};

Button::Button(QWidget *parent, bool initInConstructor): Label(parent, initInConstructor),
        running(), selected(), hasStyle(true), activatedOnPress(), hasImg(),
        hasFocus(true), type(Click) {
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void Button::setButtonText(const QString &text) {
    hasImg = false;
    setText(text);
    setSizeToText();
}

void Button::setButtonImg(const QPixmap &p1, const QPixmap& p2) {
    hasImg = true;
    imgNormal = p1;
    imgDisabled = p2.isNull() ? p1 : p2;
    auto p = isEnabled() ? imgNormal : imgDisabled;
    setFixedSize(p.size());
    setPixmap(p);
}

void Button::setButtonStyleEnabled(bool e) {
    if (hasStyle != e) {
        hasStyle = e;
        if (e) {
            setStyleSheet(QSS_NORMAL);
        }
    }
}

void Button::setSelected(bool s) {
    if (!enabled || selected == s || type == Click) {
        return;
    }
    selected = s;
    if (hasStyle && type == Select) {
        if (s) {
            setStyleSheet(QSS_SELECTED);
        } else if (Helpers::isMouseHovered(this)) {
            setStyleSheet(QSS_HOVERED);
        } else {
            setStyleSheet(QSS_NORMAL);
        }
    }
    emit sigSelected(s);
}

void Button::onPostParsing(Handlers &handlers, NBT *widgetTag) {
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
        handlers << [img](QWidget *target) {
            static_cast<Button*>(target)->setButtonImg(img);
        };
    }
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
    if (widgetTag->contains("focus", Data::BOOL)) {
        bool hasFocus = widgetTag->getBool("focus");
        handlers << [hasFocus](QWidget *w) {
            static_cast<Button*>(w)->setHasFocus(hasFocus);
        };
    }
}

void Button::setWidgetEnabled(bool e) {
    if (enabled == e) {
        return;
    }
    enabled = e;
    if (!hasStyle) {
        setStyleSheet(QSS_NORMAL);
        return;
    }
    if (enabled) {
        if (hasImg) {
            setPixmap(imgNormal);
            setFixedSize(imgNormal.size());
            if (type == Select) {
                if (selected) {
                    setStyleSheet(QSS_SELECTED);
                } else if (Helpers::isMouseHovered(this)) {
                    setStyleSheet(QSS_HOVERED);
                }
            } else if(Helpers::isMouseHovered(this)) {
                setStyleSheet(QSS_HOVERED);
            }
        } else {
            auto p = palette();
            p.setColor(QPalette::WindowText, Styles::GRAY_TEXT_1->color);
            p.setColor(QPalette::Text, Styles::GRAY_TEXT_1->color);
            setPalette(p);
        }
    } else {
        if (type == Select && selected) {
            setStyleSheet(QSS_SELECTED_DISABLED);
        } else {
            setStyleSheet(QSS_NORMAL);
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

void Button::mainInit() {
    QSS_NORMAL = bg(Styles::CLEAR->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px");
    QSS_HOVERED = bg(Styles::GRAY_1->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px");
    QSS_PRESSED = bg(Styles::GRAY_2->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px");
    QSS_SELECTED = bg(Styles::GRAY_2->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px");
    QSS_SELECTED_DISABLED = bg(Styles::CLEAR->rgbHex) + bd("2px", "solid", Styles::GRAY_1->rgbHex) + brad("2px");
}

void Button::enterEvent(QEnterEvent *event) {
    if (!enabled) {
        return;
    }
    if (hasStyle && (type != Select || !selected)) {
        setStyleSheet(QSS_HOVERED);
    }
    QWidget::enterEvent(event);
}

void Button::mouseReleaseEvent(QMouseEvent *ev) {
    if (!enabled) {
        return;
    }
    if (!activatedOnPress) {
        handleButtonActivate(ev);
    }
    QWidget::mouseReleaseEvent(ev);
}

void Button::mousePressEvent(QMouseEvent *ev) {
    if (!enabled) {
        return;
    }
    if (hasStyle) {
        setStyleSheet(QSS_PRESSED);
    }
    if (hasFocus) {
        FocusManager::mark(this);
    }
    if (activatedOnPress) {
        handleButtonActivate(ev);
    }
    ev->ignore();
}

void Button::leaveEvent(QEvent *event) {
    if (!enabled) {
        return;
    }
    if (hasStyle && (type != Select || !selected)) {
        setStyleSheet(QSS_NORMAL);
    }
    QWidget::leaveEvent(event);
}

void Button::handleButtonActivate(QMouseEvent *ev) {
    if (!Helpers::isHovered(this, ev)) {
        return;
    }
    if (hasStyle && (type == Click || type == SelectClick)) {
        setStyleSheet(QSS_HOVERED);
    }
    if (type == Click) {
        emit sigActivated();
    } else {
        setSelected(!selected);
    }
}
