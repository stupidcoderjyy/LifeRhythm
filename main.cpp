
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "IconTextButton.h"
#include "ModuleLog.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>

#include "NestedListData.h"
#include "RangeBar.h"
#include "Calendar.h"

USING_NAMESPACE(lr)

int main(const int argc, char *argv[]) {
    auto* lr = new LifeRhythm(argc, argv);
    auto cfg = lr->getConfig();
    cfg.setMode(Config::Normal);
    lr->setConfig(cfg);
    lr->registerModule(new log::ModuleLog());
    lr->onPostInit([] {
//        auto* c = new Calendar(new WeekDayTitleDrawer, new CalendarContentDrawer);
//        c->show();
    });
    return lr->launch();
}