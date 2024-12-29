//
// Created by JYY on 24-10-27.
//

#ifndef TICKHANDLER_H
#define TICKHANDLER_H

#include <QDateTime>
#include <QTimer>

#include "Namespaces.h"
#include "Plugin.h"

BEGIN_NAMESPACE(lr)

class CORE_API ITickListener {
public:
    virtual ~ITickListener();
    virtual void update(const QDateTime& now);
};

class CORE_API TickHandler {
    friend class LifeRhythm;
private:
    bool started;
    int intervalMSec;
    QTimer timer;
    QVector<ITickListener*> listeners;
public:
    void registerListener(ITickListener* listener);
    void unregisterListener(ITickListener* listener);
    void start();
    void stop();
    ~TickHandler();
private:
    explicit TickHandler(int intervalMSec);
};

END_NAMESPACE

#endif //TICKHANDLER_H
