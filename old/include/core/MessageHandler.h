//
// Created by JYY on 24-10-27.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include "NBT.h"
#include "Plugin.h"

class CORE_API Message {
    friend class MessageHandler;
private:
    Identifier id;
public:
    explicit Message(Identifier id);
    virtual void onReplied(int result);
    virtual NBT* buildData();
    virtual ~Message();
};

class CORE_API MessageListener {
    friend class MessageHandler;
public:
    static constexpr int SUCCESS = 0;
    static constexpr int FAILED = 1;
public:
    virtual int onReceived(const Identifier& sender, const NBT* data);
    virtual ~MessageListener();
};

class CORE_API MessageHandler final : public QObject {
private:
    QMap<Identifier, QVector<MessageListener*>*> listeners;
public:
    void sendMessage(Message* message) const;
    void sendDirectMessage(Message* message) const;
    void registerListener(const Identifier& sender, MessageListener* listener);
    void registerListener(const Identifier& sender, std::function<int(const Identifier& sender, const NBT* data)> func);
    void unregisterListeners(const Identifier& sender) const;
    ~MessageHandler() override;
};

class CORE_API FuncListener : public MessageListener {
private:
    std::function<int(const Identifier& sender, const NBT* data)> func;
public:
    explicit FuncListener(std::function<int(const Identifier& sender, const NBT* data)> func);
    int onReceived(const Identifier &sender, const NBT *data) override;
};



#endif //MESSAGE_H
