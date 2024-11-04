//
// Created by JYY on 24-10-30.
//

#ifndef NBTASYNCGETTER_H
#define NBTASYNCGETTER_H

#include <QFutureInterface>
#include "NBT.h"

class CORE_API NBTAsyncGetter : public QObject, public QRunnable {
    Q_OBJECT
private:
    std::function<NBT*()> builder;
    NBT* result;
    bool hasConnection;
public:
    explicit NBTAsyncGetter(std::function<NBT*()> builder);
    void start();
    void onFinished(const QObject* target, const std::function<void(NBT*)>& func);
    void run() override;
    ~NBTAsyncGetter() override;
signals:
    void sigTaskFinished(NBT*);
};



#endif //NBTASYNCGETTER_H
