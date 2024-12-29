
#include <qboxlayout.h>
#include <RcManagers.h>

#include "ArrowButton.h"
#include "Button.h"
#include "MiniCalendar.h"
#include "LifeRhythm.h"
#include "TextButton.h"

USING_NAMESPACE(lr)

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onPostInit([] {
        auto parent = new QWidget;
        parent->setStyleSheet(bg(Styles::BLACK->rgbHex));
        auto layout = new QHBoxLayout(parent);
        layout->setSpacing(0);
        layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        parent->setLayout(layout);

        auto b1 = new ArrowButton(parent);
        b1->setBaseType(ArrowButton::Right);
        b1->setActivatedType(ArrowButton::Down);
        b1->setButtonStyleEnabled(false);
        b1->setActivateOnPress(true);

        auto b2 = new Button(parent);
        b2->setButtonText("测试");
        layout->addWidget(b1);
        layout->addWidget(b2);
        parent->show();
    });
    return lr.launch();
}
