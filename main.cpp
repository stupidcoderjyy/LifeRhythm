
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "OpenRange.h"
#include <QDebug>
#include <qthread.h>
#include <utility>

#include "NBT.h"

USING_NAMESPACE(lr)

int runLifeRhythm(const int argc, char *argv[]) {
    auto* lr = new LifeRhythm(argc, argv);
    auto cfg = lr->getConfig();
    cfg.setMode(Config::Normal);
    lr->setConfig(cfg);
    return lr->launch();
}

class TestMessage : public Message {
public:
    explicit TestMessage(const Identifier &id)
        : Message(id) {
    }

    void onReplied(int result) override {
        qDebug() << result;
    }

    NBT * buildData() override {
        auto nbt = new NBT;
        QThread::msleep(3000);
        nbt->putString("a")->setVal("testA");
        nbt->putString("b")->setVal("testB");
        return nbt;
    }
};

class Listener : public MessageListener {
public:
    QString key;
public:
    explicit Listener(QString key)
        : key(std::move(key)) {
    }

    int onReceived(const Identifier &sender, const NBT *data) override {
        qDebug() << "received: " + data->getString(key);
        return SUCCESS;
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MessageHandler handler;
    auto loc = LOC("test:sender");
    handler.registerListener(loc, new Listener("a"));
    handler.registerListener(loc, new Listener("b"));
    handler.sendMessage(new TestMessage(loc));
    qDebug() << "message sent";
    return QApplication::exec();
}