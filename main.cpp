
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "IconTextButton.h"
#include "ModuleLog.h"
#include <QVBoxLayout>
#include <QDebug>

USING_NAMESPACE(lr)

int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    auto cfg = lr.getConfig();
    cfg.setMode(Config::Normal);
    lr.setConfig(cfg);
    lr.registerModule(new log::ModuleLog());
    return lr.launch();
}
