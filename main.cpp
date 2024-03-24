
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "WidgetDataStorage.h"
#include "PeriodType.h"
#include "Styles.h"
#include "RcManagers.h"
#include "MainTab.h"
#include "LineEdit.h"
#include "ColorSelector.h"
#include <QTimer>

USING_LR

int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    auto cfg = lr.getConfig();
    cfg.setMode(Config::Normal);
    lr.setConfig(cfg);
    lr.onMainInit([](){
    });
    lr.onPostInit([](){
        auto* selector = WidgetFactoryStorage::get("lr:widget_hue_selector")->applyAndCast<HueSelector>();
        selector->show();
        QObject::connect(selector, &HueSelector::sigColorSelected, [](const QColor& c){
            qDebug() << c;
        });
    });
    return lr.launch();
}
