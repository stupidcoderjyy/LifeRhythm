//
// Created by JYY on 24-12-12.
//

#include "AsyncGetter.h"
#include <QThreadPool>

USING_LR

AsyncGetter * AsyncGetter::create() {
    return new AsyncGetter();
}

void AsyncGetter::start() {
    QThreadPool::globalInstance()->start(this);
}

void AsyncGetter::setBuilder(std::function<QObject*()> b) {
    builder = std::move(b);
}

void AsyncGetter::setHandler(const QObject* receiver, const std::function<void(QObject*)> &f) {
    connect(this, &AsyncGetter::sigTaskFinished, this, [f](QObject* o) {
        f(o);
        delete o;
    }, Qt::QueuedConnection);
    hasConnection = true;
}

void AsyncGetter::run() {
    result = builder();
    emit sigTaskFinished(result);
}

AsyncGetter::~AsyncGetter() {
    if (!hasConnection) {
        delete result;
    }
}

AsyncGetter::AsyncGetter(): result(), hasConnection() {
    setAutoDelete(true);
}
