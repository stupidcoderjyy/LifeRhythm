#include <qboxlayout.h>
#include <qthread.h>
#include <RcManagers.h>
#include "LifeRhythm.h"
#include "SlotsPainter.h"
#include "ListWidget.h"
#include "Calendar.h"
#include "DropDown.h"
#include "LoadingIcon.h"

USING_NAMESPACE(lr)

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([] {
        Button::mainInit();
        Calendar::mainInit();
    });
    lr.onPostInit([] {
        auto parent = new QWidget;
        parent->setObjectName("parent");
        parent->setStyleSheet(qss_target("#parent", bg(Styles::GRAY_0->rgbHex)));
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);

        auto icon = new LoadingIcon;
        icon->startLoading();

        layout->addWidget(icon);
        QTimer::singleShot(0, [parent] {
            parent->show();
        });
    });
    return lr.launch();
}
