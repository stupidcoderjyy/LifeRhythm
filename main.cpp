
#include "Namespaces.h"
#include "LifeRhythm.h"

USING_NAMESPACE(lr)

int runLifeRhythm(const int argc, char *argv[]) {
    auto* lr = new LifeRhythm(argc, argv);
    auto cfg = lr->getConfig();
    cfg.setMode(Config::Normal);
    lr->setConfig(cfg);
    return lr->launch();
}

int main(int argc, char *argv[]) {
}