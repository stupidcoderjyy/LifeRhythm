//
// Created by stupid_coder_jyy on 2024/1/14.
//

#include "StandardWidget.h"
#include "MemUtil.h"
#include <QDebug>

void StandardWidget::onPreParsing(Handlers &handlers, NBT *widgetTag) {
}

void StandardWidget::onPostParsing(Handlers &handlers, NBT *widgetTag) {
}

void StandardWidget::onFinishedParsing(Handlers &handlers, NBT *widgetTag) {
}

void StandardWidget::onStateRespondersParsing(Handlers &responders, NBT *stateTag) {
}

int StandardWidget::getState() const {
    return state;
}

void StandardWidget::registerResponder(int _state, const StandardWidget::Handler &responder) {
    if (stateResponders.contains(_state)) {
        stateResponders.value(_state)->append(responder);
    } else {
        auto* res = new Handlers();
        res->append(responder);
        stateResponders.insert(_state, res);
    }
}

void StandardWidget::registerGlobalResponder(const StandardWidget::Handler &responder) {
    globalResponders << responder;
}

void StandardWidget::setState(int newState) {
    if (state == newState) {
        return;
    }
    auto* _this = dynamic_cast<QWidget*>(this);
    state = newState;
    for (auto& r : globalResponders) {
        r(_this);
    }
    auto* responders = stateResponders.value(state);
    if (responders) {
        for (auto& r : *responders) {
            r(_this);
        }
    }
}

void StandardWidget::registerPointer(const QString &id, QWidget *p) {
    pointers.insert(id, p);
}

StandardWidget::~StandardWidget() {
    DELETE_MAP(stateResponders)
}
