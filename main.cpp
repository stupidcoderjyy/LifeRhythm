
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "OpenRange.h"
#include <QDebug>

#include "NBT.h"

USING_NAMESPACE(lr)

int runLifeRhythm(const int argc, char *argv[]) {
    auto* lr = new LifeRhythm(argc, argv);
    auto cfg = lr->getConfig();
    cfg.setMode(Config::Normal);
    lr->setConfig(cfg);
    return lr->launch();
}

int main(const int argc, char *argv[]) {
    auto nbt = NBT::fromStringNbt("rule.txt");
    qDebug() << nbt->toString();
    qDebug() << nbt->getLong("l1");
}