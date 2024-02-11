//
// Created by stupid_coder_jyy on 2024/2/9.
//

#include "LifeRhythm.h"
#include "WidgetFactoryStorage.h"
#include "ImageStorage.h"
#include "MainFrame.h"
#include "TabBar.h"
#include "IOUtil.h"
#include <QtConcurrent>

LifeRhythm* LifeRhythm::lr{};

LifeRhythm *LifeRhythm::get() {
    return lr;
}

void LifeRhythm::launch() {
    (lr = new LifeRhythm())->launch0();
}

void LifeRhythm::launch0() {
    preInit();
    emit sigPreInit();
    auto* watcher = new QFutureWatcher<void>(this);
    watcher->setFuture(QtConcurrent::run([this]() {
        mainInit();
        emit sigMainInit();
        postInit();
        emit sigPostInit();
    }));
}

void LifeRhythm::preInit() {
}

void LifeRhythm::mainInit() {
    ImageStorage::getInstance()->init("lr");
    WidgetFactoryStorage::getInstance()->init("lr");
    WidgetFactory::init();
    TabBar::mainInit();
    debug("init finished");
}

void LifeRhythm::postInit() {
    WidgetFactoryStorage::getInstance()->parseAll();
}

LifeRhythm::LifeRhythm():QObject() {
    connect(this, &LifeRhythm::sigPreInit, [this](){
        mainFrame = new MainFrame();
    });
    connect(this, &LifeRhythm::sigPostInit, this, [this](){
        mainFrame->tabBar->insertTab("LifeRhythm 你好世界", new TabWidget());
    }, Qt::QueuedConnection);
}
