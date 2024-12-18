//
// Created by stupid_coder_jyy on 2024/3/3.
//

#include "Task.h"
#include <utility>
#include <QThreadPool>
#include <functional>

Task::Task(std::function<void(QFutureInterface<void>&)> task): task(std::move(task)) {
    futureWatcher.setFuture(futureInterface.future());
    setAutoDelete(true);
}

QFuture<void> Task::future() {
    return futureInterface.future();
}

const QFutureWatcher<void>* Task::getFutureWatcher() const {
    return &futureWatcher;
}

void Task::start() {
    QThreadPool::globalInstance()->start(this);
}

void Task::onFinished(const QObject *target, std::function<void()> func) const {
    connect(this, &Task::sigTaskFinished, target, std::move(func), Qt::QueuedConnection);
}

void Task::onProgressChanged(const QObject *target, const std::function<void(int)> &func) {
    connect(&futureWatcher, &QFutureWatcher<void>::progressValueChanged, target, func, Qt::QueuedConnection);
}

void Task::run() {
    futureInterface.reportStarted();
    task(futureInterface);
    futureInterface.reportFinished();
    emit sigTaskFinished();
}
