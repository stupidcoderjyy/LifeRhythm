//
// Created by JYY on 24-10-30.
//

#include "NBTAsyncGetter.h"
#include <QThreadPool>
#include <utility>

#include "Error.h"

NBTAsyncGetter::NBTAsyncGetter(std::function<NBT*()> builder): builder(std::move(builder)), result() {
    setAutoDelete(true);
}

void NBTAsyncGetter::start() {
    QThreadPool::globalInstance()->start(this);
}

void NBTAsyncGetter::onFinished(const QObject* target, const std::function<void(NBT*)>& func) {
    connect(this, &NBTAsyncGetter::sigTaskFinished, target, [func](NBT* nbt) {
        func(nbt);
        delete nbt;
    }, Qt::QueuedConnection);
    hasConnection = true;
}

void NBTAsyncGetter::run() {
    result = builder();
    emit sigTaskFinished(result);
}

NBTAsyncGetter::~NBTAsyncGetter() {
    if (!hasConnection) {
        delete result;
    }
}
