
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "WidgetDataStorage.h"
#include "PeriodType.h"
#include "Styles.h"
#include "RcManagers.h"
#include "MainTab.h"
#include "LineEdit.h"
#include <QTimer>

USING_LR

int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    auto cfg = lr.getConfig();
    cfg.setMode(Config::Normal);
    lr.setConfig(cfg);
    lr.onMainInit([](){
        auto* tree = new SelectableTreeData();
        for (char c = 'a' ; c < 'c' ; c ++) {
            QString text = QString(c);
            auto* t = new PeriodType(Styles::CYAN_0, text);
            for (char cc = 'a' ; cc < 'z' ; cc++) {
                auto* tt = new PeriodType(Styles::CYAN_0, text + cc);
                t->addChildren(tree, tt);
//                for (char ccc = 'a' ; ccc < 'd' ; ccc++) {
//                    auto* ttt = new PeriodType(Styles::CYAN_0, text + cc + ccc);
//                    tt->addChildren(tree, ttt);
//                }
            }
            tree->addNode(t);
        }
        WidgetDataStorage::add("log:period_type", tree);
        WidgetDataStorage::add("log:period_type_example", new PeriodType(Styles::CYAN_0, "c"));
        MainTab::mainInit();
    });
    lr.onPostInit([](){
        LifeRhythm::insertTab("a", "log:widget_maintab");
    });
    return lr.launch();
}
