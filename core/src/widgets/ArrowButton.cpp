//
// Created by stupid_coder_jyy on 2024/2/28.
//

#include "ArrowButton.h"
#include "Styles.h"
#include "NBT.h"
#include "Error.h"
#include <QResizeEvent>
#include <QPainter>

QMap<QString, ArrowButton::Type> ArrowButton::directions{};

#define reg(t) directions.insert(#t, t);

void ArrowButton::mainInit() {
    reg(Up)
    reg(Down)
    reg(Left)
    reg(Right)
}

ArrowButton::ArrowButton(QWidget *parent): Widget(parent), type(Down), activatedType(Down),
        ratioWidth(0.75), ratioHeight(0.4), pressed(),
        points(), scale(1.0) {
}

void ArrowButton::setActivatedType(ArrowButton::Type t) {
    activatedType = t;
    if (pressed) {
        updatePoints(t, false);
    }
}

void ArrowButton::setBaseType(ArrowButton::Type t) {
    type = t;
    if (!pressed) {
        updatePoints(t, false);
    }
}

void ArrowButton::setRatioWidth(double rw) {
    ratioWidth = rw;
}

void ArrowButton::setRatioHeight(double rh) {
    ratioHeight = rh;
}

void ArrowButton::setRatio(double rw, double rh) {
    ratioWidth = rw;
    ratioHeight = rh;
}

void ArrowButton::setScale(double s) {
    scale = s;
}

void ArrowButton::setPressed(bool p) {
    if (pressed != p) {
        updatePoints(p ? activatedType : type);
        pressed = p;
    }
}

void ArrowButton::onPostParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    if (widgetTag->contains("directions", Data::ARR)) {
        QStringList list;
        widgetTag->get("directions")->asArray()->fillString(list);
        if (list.empty()) {
            throwInFunc("property directions in ArrowButton requires QString[1] or QString[2]");
        }
        if (!directions.contains(list[0])) {
            throwInFunc("unknown directions '" + list[0] + "' in ArrowButton");
        }
        if (list.length() > 1 && !directions.contains(list[1])) {
            throwInFunc("unknown directions '" + list[1] + "' in ArrowButton");
        }
        handlers << [list](QWidget* target) {
            auto* btn = static_cast<ArrowButton*>(target);
            if (list.length() == 1) {
                Type t = directions.value(list[0]);
                btn->setBaseType(t);
                btn->setActivatedType(t);
            } else {
                btn->setBaseType(directions.value(list[0]));
                btn->setActivatedType(directions.value(list[1]));
            }
        };
    }
    if (widgetTag->contains("ratio", Data::ARR)) {
        float ratio[2]{-1, -1};
        widgetTag->get("ratio")->asArray()->fillFloat(ratio, 2);
        if (ratio[0] <= 0 || ratio[1] <= 0) {
            throwInFunc("invalid ratio array in ArrowButton, requires float[2] and positive elements");
        }
        handlers << [ratio](QWidget* target) {
            static_cast<ArrowButton*>(target)->setRatio(ratio[0], ratio[1]);
        };
    }
    if (widgetTag->contains("scale", Data::FLOAT)) {
        float s = widgetTag->getFloat("scale");
        handlers << [s](QWidget* target) {
            static_cast<ArrowButton*>(target)->setScale(s);
        };
    }
    handlers << [](QWidget* target) {
        auto* b = static_cast<ArrowButton*>(target);
        b->updatePoints(b->type, false);
    };
}

void ArrowButton::paintEvent(QPaintEvent *event) {
    if (type == None) {
        return;
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(Styles::GRAY_TEXT_0->color);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(points[0], points[1], points[2], points[3]);
    painter.drawLine(points[2], points[3], points[4], points[5]);
    painter.setRenderHint(QPainter::Antialiasing, false);
}

void ArrowButton::mousePressEvent(QMouseEvent *event) {
    if (type == None) {
        event->ignore();
        return;
    }
    setPressed(!pressed);
    emit sigActivated(pressed);
}

void ArrowButton::updatePoints(Type t, bool u) {
    if (t == None) {
        return;
    }
    auto s = size();
    int w0 = qRound(s.width() * ratioWidth * scale);
    int h0 = qRound(s.height() * ratioHeight * scale);
    if (t & 2) {
        int w = h0;
        int h = w0;
        points[1] = (s.height() - h) >> 1;
        points[3] = s.height() >> 1;
        points[5] = s.height() - points[1];
        if (t == Right) {
            points[0] = (s.width() - w) >> 1;
            points[2] = s.width() - points[0];
        } else {
            points[2] = (s.width() - w) >> 1;
            points[0] = s.width() - points[2];
        }
        points[4] = points[0];
    } else {
        int h = h0;
        int w = w0;
        points[0] = (s.width() - w) >> 1;
        points[2] = s.width() >> 1;
        points[4] = s.width() - points[0];
        if (t == Up) {
            points[3] = (s.height() - h) >> 1;
            points[1] = s.height() - points[3];
        } else {
            points[1] = (s.height() - h) >> 1;
            points[3] = s.height() - points[1];
        }
        points[5] = points[1];
    }
    if (u) {
        update();
    }
}
