
#include <qboxlayout.h>

#include "Label.h"
#include "LifeRhythm.h"
#include "RcManagers.h"

USING_NAMESPACE(lr)

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onPostInit([] {
        auto parent = new QWidget;
        parent->setStyleSheet(bg(Styles::BLACK->rgbHex));
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);

        auto l1 = new Label(parent);
        l1->setPixmap(*ImageStorage::get("lr:icon_30"));
        l1->setStyleSheet(bg(Styles::RED->rgbHex));

        auto l2 = new Label(parent);
        l2->setText("测试测试");
        l2->setStyleSheet(bg(Styles::CYAN_0->rgbHex));

        layout->addWidget(l1);
        layout->addWidget(l2);
        parent->show();
    });
    return lr.launch();
}
