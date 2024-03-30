
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "IconTextButton.h"
#include <QVBoxLayout>
#include <QDebug>

USING_LR

int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    auto cfg = lr.getConfig();
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([](){
    });
    lr.onPostInit([](){
        auto* root = new QWidget;
        root->setObjectName("root");
        root->setStyleSheet(qss_target("root", bg(Styles::BLACK->rgbHex)));
        root->setFixedSize(300,300);
        auto* layout = new QVBoxLayout(root);
        root->setLayout(layout);
        auto* b1 = new IconTextButton(root);
        auto* b2 = new IconTextButton(root);
        QObject::connect(b1, &IconTextButton::sigActivated, [](){
           qDebug() << "activated b1";
        });
        QObject::connect(b2, &IconTextButton::sigActivated, [](){
            qDebug() << "activated b2";
        });
        b1->set("lr:plus_30x30", "测试1");
        b2->set("lr:rollback_30x30", "测试2");
        layout->addWidget(b1);
        layout->addWidget(b2);
        root->show();
    });
    return lr.launch();
}
