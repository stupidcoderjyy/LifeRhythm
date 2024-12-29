//
// Created by JYY on 24-10-27.
//

#include "MessageHandler.h"
#include <utility>
#include <windows.h>

#include "Error.h"
#include "MemUtil.h"
#include "NBTAsyncGetter.h"
#include "PrintErrorHandler.h"

Message::Message(Identifier id): id(std::move(id)) {
}

void Message::onReplied(int result) {
}

NBT* Message::buildData() {
    return nullptr;
}

Message::~Message() = default;

int MessageListener::onReceived(const Identifier &sender, const NBT* data) {
    return SUCCESS;
}

MessageListener::~MessageListener() = default;

void MessageHandler::sendMessage(Message* message) const {
    auto list = listeners.value(message->id);
    if (!list) {
        throwInFunc("unregistered message: " + message->id.toString());
    }
    auto task = new NBTAsyncGetter([message] {
        return message->buildData();
    });
    task->onFinished(this, [message, list](const NBT* nbt) {
        for (auto listener : *list) {
            try {
                int res = listener->onReceived(message->id, nbt);
                qDebug() << res;
                message->onReplied(res);
            } catch (Error& e) {
                e.why = "error occurred when handling message from " + message->id.toString() + ":" + e.why;
                PrintErrorHandler().onErrorCaught(e);
                message->onReplied(MessageListener::FAILED);
            }
        }
        delete message;
    });
    task->start();  //auto delete
}

void MessageHandler::sendDirectMessage(Message *message) const {
    auto list = listeners.value(message->id);
    if (!list) {
        throwInFunc("unregistered message: " + message->id.toString());
    }
    auto nbt = message->buildData();
    for (auto listener : *list) {
        try {
            int res = listener->onReceived(message->id, nbt);
            message->onReplied(res);
        } catch (Error& e) {
            e.why = "error occurred when handling message from " + message->id.toString() + ":" + e.why;
            PrintErrorHandler().onErrorCaught(e);
        }
    }
    delete message;
}

void MessageHandler::registerListener(const Identifier& sender, MessageListener *listener) {
    auto* list = listeners.value(sender);
    if (!list) {
        list = new QVector<MessageListener*>();
        listeners.insert(sender, list);
    }
    list->append(listener);
}

void MessageHandler::registerListener(const Identifier &sender, std::function<int(const Identifier &sender, const NBT *data)> func) {
    registerListener(sender, new FuncListener(std::move(func)));
}

void MessageHandler::unregisterListeners(const Identifier &sender) const {
    auto* list = listeners.value(sender);
    if (!list) {
        return;
    }
    DELETE_LIST_P(list)
}

MessageHandler::~MessageHandler() {
    for (const auto& list : listeners) {
        DELETE_LIST_P(list)
        delete list;
    }
}

FuncListener::FuncListener(std::function<int(const Identifier &sender, const NBT *data)> func): func(std::move(func)) {
}

int FuncListener::onReceived(const Identifier &sender, const NBT *data) {
    return func(sender, data);
}
