//
// Created by stupid_coder_jyy on 2024/3/3.
//

#ifndef LIFERHYTHM_TASK_H
#define LIFERHYTHM_TASK_H

#include <QRunnable>
#include <QString>
#include <QFutureInterface>
#include <QFuture>
#include <QFutureWatcher>

class Task : public QObject, public QRunnable{
    Q_OBJECT
protected:
    QFutureInterface<void> futureInterface;
    std::function<void(QFutureInterface<void>&)> task;
    QFutureWatcher<void> futureWatcher;
public:
    explicit Task(std::function<void(QFutureInterface<void>& fi)> task);
    QFuture<void> future();
    const QFutureWatcher<void>* getFutureWatcher();
    void start();
    void onFinished(QObject* target, std::function<void()> func) const;
    void onProgressChanged(QObject* target, const std::function<void(int)> &func);
signals:
    void sigTaskFinished();
private:
    void run() override;
};

#endif //LIFERHYTHM_TASK_H
