
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "IconTextButton.h"
#include "ModuleLog.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>

#include "NestedListData.h"
#include "RangeBarList.h"

USING_NAMESPACE(lr)

int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    auto cfg = lr.getConfig();
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.registerModule(new log::ModuleLog());
    lr.onPostInit([](){
        auto* d = new NestedListData;
        auto* list = new RangeBarList(new HBarListContainer);
        list->setWindowFlag(Qt::FramelessWindowHint, true);
        list->setFixedSize(400, 400);
        list->setBarRange(0, 24 * 60);
        list->setMainZoomRange(1.0, 2.0);
        list->setMainZoomStep(0.2);
        list->setSideZoomRange(40, 60);
        list->setSideZoomStep(2);
        list->setData(d);
        d->beginEdit();
        auto* bd = new BarData(0 * 60, 4 * 60);
        d->append(0, bd);
        d->endEdit();
        list->show();
        QTimer::singleShot(3000, [bd](){
            bd->changeRow(1);
            bd->setBegin(2 * 60);
            bd->setEnd(3 * 60);
            emit bd->sigDataChanged();
        });
    });
    return lr.launch();
}