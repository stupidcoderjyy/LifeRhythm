
#include <qboxlayout.h>
#include <RcManagers.h>

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
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);

        auto b1 = new Button(parent);
        b1->setButtonText("测试");
        QObject::connect(b1, &Button::sigActivated, [] {
            qDebug() << "activated";
        });

        auto b2 = new Button(parent);
        b2->setButtonImg(*ImageStorage::get("lr:plus_30x30"), *ImageStorage::get("lr:minus_30x30"));
        b2->setButtonMode(Button::Select);
        b2->setSelected(true);
        QObject::connect(b2, &Button::sigSelected, [] {
            qDebug() << "selected";
        });
        QObject::connect(b2, &Button::sigCancelled, [] {
            qDebug() << "cancelled";
        });

        auto b3 = new Button(parent);
        b3->setButtonImg(*ImageStorage::get("lr:plus_30x30"), *ImageStorage::get("lr:minus_30x30"));
        b3->setButtonMode(Button::SelectClick);
        QObject::connect(b3, &Button::sigSelected, [] {
            qDebug() << "selected";
        });
        QObject::connect(b3, &Button::sigCancelled, [] {
            qDebug() << "cancelled";
        });

        QTimer::singleShot(1000, [b1, b2, b3] {
            b1->setButtonEnabled(false);
            b2->setButtonEnabled(false);
            b3->setButtonEnabled(false);
        });
        QTimer::singleShot(5000, [b1, b2, b3] {
            b1->setButtonEnabled(true);
            b2->setButtonEnabled(true);
            b3->setButtonEnabled(true);
        });


        layout->addWidget(b1);
        layout->addWidget(b2);
        layout->addWidget(b3);
        parent->show();
    });
    return lr.launch();
}
