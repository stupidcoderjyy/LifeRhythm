//
// Created by JYY on 24-12-12.
//

#ifndef ASYNCGETTER_H
#define ASYNCGETTER_H

#include <QObject>
#include <QRunnable>

#include "Namespaces.h"

BEGIN_LR

class AsyncGetter : public QObject, public QRunnable {
    Q_OBJECT
private:
    std::function<QObject*()> builder;
    QObject* result;
    bool hasConnection;
public:
    static AsyncGetter* create();
    void start();
    void setBuilder(std::function<QObject*()> builder);
    void setHandler(const QObject *receiver, const std::function<void(QObject*)> &func);
    void run() override;
    ~AsyncGetter() override;
signals:
    void sigTaskFinished(QObject*);
private:
    explicit AsyncGetter();
};

END_NP

#endif //ASYNCGETTER_H
