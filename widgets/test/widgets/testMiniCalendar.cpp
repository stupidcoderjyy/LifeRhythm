
#include <qboxlayout.h>
#include <RcManagers.h>

#include "MiniCalendar.h"
#include "LifeRhythm.h"

USING_NAMESPACE(lr)
using ViewLevel = mini_calendar::ViewLevel;

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([] {
        auto f = WidgetFactoryStorage::get("test:widget_mini_calendar");
        regClazz(f, Button);
        regClazz(f, ArrowButton);
        regClazz(f, Widget);
        Button::mainInit();
    });
    lr.onPostInit([] {
        auto parent = new QWidget;
        auto layout = new QHBoxLayout(parent);

        auto mc1 = new MiniCalendar(parent);
        QTimer::singleShot(1000, [mc1] {
            mc1->setMaxViewLevel(ViewLevel::Month);
        });

        auto mc2 = new MiniCalendar(parent);
        mc2->setMaxViewLevel(ViewLevel::Year);
        QTimer::singleShot(2000, [mc2] {
            mc2->setMaxViewLevel(ViewLevel::Day);
        });

        auto mc3 = new MiniCalendar(parent);
        mc3->setMaxViewLevel(ViewLevel::Month);

        layout->addWidget(mc1);
        layout->addWidget(mc2);
        layout->addWidget(mc3);
        parent->setLayout(layout);
        parent->show();
    });
    return lr.launch();
}
