
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "WidgetDataStorage.h"
#include "PeriodType.h"
#include "Styles.h"
#include "RcManagers.h"
#include "PeriodTypeTreeWidget.h"

USING_LR

int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    auto cfg = lr.getConfig();
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([](){
        auto* tree = new SelectableTreeData();
        auto* a = new PeriodType(Styles::GOLD, "测试a");
        auto* aa = new PeriodType(Styles::GOLD, "测试aa");
        auto* aaa = new PeriodType(Styles::GOLD, "测试aaa");
        auto* aab = new PeriodType(Styles::GOLD, "测试aab");
        auto* b = new PeriodType(Styles::GREEN, "测试b");
        auto* ba = new PeriodType(Styles::GREEN, "测试ba");
        a->addChildren(tree, aa);
        aa->addChildren(tree, aaa);
        aa->addChildren(tree, aab);
        b->addChildren(tree, ba);
        tree->addNode(b);
        tree->addNode(a);
        WidgetDataStorage::add("log:period_type", tree);

        QTimer::singleShot(5000, [tree, aa](){
//            aa->addChildren(tree, new PeriodType(Styles::GOLD, "测试aac"));
            aa->removeChildren(0);
        });

        regClazz(WidgetFactoryStorage::get("log:widget_mainpage"), PeriodTypeTreeWidget);
        auto* f = WidgetFactoryStorage::get("log:item_period_type");
        regClazz(f, PeriodTypeIcon);
        regClazz(f, PeriodTypeTreeItem);
    });
    lr.onPostInit([](){
        WidgetFactoryStorage::get("log:widget_mainpage")
                ->apply()
                ->show();
//        LifeRhythm::insertTab("a", "log:widget_maintab");
    });
    return lr.launch();
}
