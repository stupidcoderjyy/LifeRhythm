
#include <qboxlayout.h>

#include "MiniCalendar.h"
#include "LifeRhythm.h"
#include "JbTextButton.h"

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

        auto btn = new JbTextButton(parent);
        btn->setButtonText("取消");
        btn->setButtonType(JbTextButton::Normal);
        btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        auto btn2 = new JbTextButton(parent);
        btn2->setButtonText("确认");
        btn2->setButtonType(JbTextButton::Highlighted);
        btn2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        auto btn3 = new JbTextButton(parent);
        btn3->setButtonText("应用");
        btn3->setButtonType(JbTextButton::Disabled);
        btn3->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        layout->addWidget(btn);
        layout->addWidget(btn2);
        layout->addWidget(btn3);
        parent->show();
    });
    return lr.launch();
}
