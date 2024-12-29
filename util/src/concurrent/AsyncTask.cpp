//
// Created by JYY on 24-12-12.
//

#include "AsyncTask.h"
#include <QThreadPool>

USING_LR

AsyncTask* AsyncTask::create() {
    return new AsyncTask();
}

void AsyncTask::start() {
    QThreadPool::globalInstance()->start(this);
}

void AsyncTask::setTask(std::function<void()> h) {
    handler = std::move(h);
}

void AsyncTask::setFinished(const QObject *receiver, const std::function<void()> &f) {
    connect(this, &AsyncTask::sigTaskFinished, receiver, [f] {
        f();
    }, Qt::QueuedConnection);
}

void AsyncTask::run() {
    handler();
    emit sigTaskFinished();
}

AsyncTask::AsyncTask() {
    setAutoDelete(true);
}
