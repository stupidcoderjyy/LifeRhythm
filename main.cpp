
#include "SelectableListWidget.h"
#include "OptionsBox.h"
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "MainTab.h"
#include "RcManagers.h"
#include "PeriodType.h"

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
        PeriodTypeLoader loader("test.txt");
        loader.load();
        auto* tree = loader.getData()->cast<PeriodTypeTree>();
        if (!tree->length()) {
            auto* t0 = new PeriodType(Styles::BLUE_0, "t0");
            auto* t00 = new PeriodType(Styles::BLUE_0, "t00");
            auto* t01 = new PeriodType(Styles::BLUE_0, "t01");
            auto* t02 = new PeriodType(Styles::BLUE_0, "t02");
            auto* t020 = new PeriodType(Styles::BLUE_0, "t020");
            auto* t0200 = new PeriodType(Styles::BLUE_0, "t0200");
            t0->addChildren(t00);
            t0->addChildren(t01);
            t0->addChildren(t02);
            t02->addChildren(t020);
            t02->setFolded(false);
            t020->addChildren(t0200);
            t020->setFolded(false);
            tree->append(t0);
            tree->foldNode(0, false);
            tree->foldNode(0, true);
        }
//        auto* tab = WidgetFactoryStorage::get("log:maintab")->apply();
//        LifeRhythm::insertTab("a", static_cast<TabWidget*>(tab));
    });
    return lr.launch();
}
