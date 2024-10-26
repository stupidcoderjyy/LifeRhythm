//
// Created by JYY on 24-10-27.
//

#include "TickHandler.h"
#include "MemUtil.h"

USING_NAMESPACE(lr)

ITickListener::~ITickListener() = default;

void ITickListener::update(const QDateTime &now) {
}

void TickHandler::registerListener(ITickListener *listener) {
    listeners << listener;
    if (started) {
        start();
    }
}

void TickHandler::unregisterListener(ITickListener *listener) {
    listeners.removeAll(listener);
}

TickHandler::~TickHandler() {
    DELETE_LIST(listeners)
}

TickHandler::TickHandler(int intervalMSec): started(), intervalMSec(intervalMSec) {
    QObject::connect(&timer, &QTimer::timeout, [this] {
        if (listeners.empty()) {
            timer.stop();
        }
        auto now = QDateTime::currentDateTime();
        for (auto listener : listeners) {
            listener->update(now);
        }
    });
}

void TickHandler::start() {
    if (!listeners.empty()) {
        timer.start(intervalMSec);
    }
    started = true;
}

void TickHandler::stop() {
    timer.stop();
    started = false;
}
