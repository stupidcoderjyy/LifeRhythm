
#include "SelectableListWidget.h"
#include "OptionsBox.h"
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "TabBar.h"
#include "MainTab.h"
#include "RcManagers.h"

#include <QThread>
#include <QFontDatabase>

USING_LR

int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    auto cfg = lr.getConfig();
    cfg.setMode(Config::Normal);
    lr.setConfig(cfg);
    lr.onMainInit([](){
        MainTab::mainInit();
    });
    lr.onPostInit([](){
        auto* tab = WidgetFactoryStorage::get("log:maintab")->apply();
        LifeRhythm::insertTab("a", static_cast<TabWidget*>(tab));
    });
    return lr.launch();
}
