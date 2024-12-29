//
// Created by stupid_coder_jyy on 2024/1/14.
//

#include "StandardWidget.h"
#include "Helpers.h"
#include <QDebug>
#include <QTimer>

USING_LR

StandardWidget::StandardWidget(bool initInConstructor): prepared() {
    if (initInConstructor) {
        QTimer::singleShot(0, dynamic_cast<QObject *>(this), [this] {
            initWidget();
        });
    }
}

void StandardWidget::registerResponder(int _state, const Handler &responder) {
    if (stateResponders.contains(_state)) {
        stateResponders.value(_state)->append(responder);
    } else {
        auto *res = new Handlers();
        res->append(responder);
        stateResponders.insert(_state, res);
    }
}

void StandardWidget::registerGlobalResponder(const Handler &responder) {
    globalResponders << responder;
}

void StandardWidget::setState(int newState) {
    if (state == newState) {
        return;
    }
    auto *_this = dynamic_cast<QWidget *>(this);
    state = newState;
    for (auto &r: globalResponders) {
        r(_this);
    }
    auto *responders = stateResponders.value(state);
    if (responders) {
        for (auto &r: *responders) {
            r(_this);
        }
    }
}

void StandardWidget::onPreParsing(Handlers &handlers, NBT *nbt) {
}

void StandardWidget::onPostParsing(Handlers &handlers, NBT *nbt) {
}

void StandardWidget::onFinishedParsing(Handlers &handlers, NBT *nbt) {
}

void StandardWidget::onStateRespondersParsing(Handlers &responders, NBT *nbt) {
}

void StandardWidget::registerPointer(const QString &id, QWidget *p) {
    pointers.insert(id, p);
}

void StandardWidget::registerPointer(const StandardWidget *other) {
    pointers.insert(other->pointers);
}

void StandardWidget::setData(WidgetData *d) {
    if (!prepared) {
        initWidget();
    }
    if (wData == d) {
        return;
    }
    if (wData) {
        for (auto &c: dc) {
            QObject::disconnect(c);
        }
    }
    wData = d;
    if (d) {
        connectModelView();
    }
    syncDataToWidget();
}

void StandardWidget::syncDataToWidget() {
}

void StandardWidget::syncWidgetToData() {
}

void StandardWidget::setWidgetEnabled(bool enabled) {
    this->enabled = enabled;
}

StandardWidget::~StandardWidget() {
    DELETE_MAP(stateResponders)
}

void StandardWidget::connectModelView() {
}

void StandardWidget::initWidget() {
    prepared = true;
}