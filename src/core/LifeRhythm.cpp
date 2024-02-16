//
// Created by stupid_coder_jyy on 2024/2/9.
//

#include "LifeRhythm.h"
#include "MainFrame.h"
#include "TabBar.h"
#include "IOUtil.h"
#include "RcManagers.h"
#include "TitledDialog.h"
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
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    Styles::initStyles();
    mainFrame = new MainFrame();
}

void LifeRhythm::mainInit() {
    ImageStorage::init();
    WidgetFactoryStorage::init();
    WidgetFactory::init();
    TabBar::mainInit();
    TitledDialog::mainInit();
    debugPrint("_init finished");
}

void LifeRhythm::postInit() {
    WidgetFactoryStorage::parseAll();
}

LifeRhythm::LifeRhythm():QObject() {
    connect(this, &LifeRhythm::sigPostInit, this, [this](){
        mainFrame->tabBar->insertTab("LifeRhythm 你好世界", new TabWidget());
        auto* label = new QLabel();
        label->setText("dwadwadwa");
        label->setFont(Styles::FONT_TEXT_EDIT);
        generateTitledDialog("测试", label);
    }, Qt::QueuedConnection);
}

void LifeRhythm::generateTitledDialog(const QString &title, QWidget *content) const {
    auto* dialog = static_cast<TitledDialog*>(WidgetFactoryStorage::get("lr:titled_dialog")->apply());
    dialog->setContent(title, content);
    connect(this, &LifeRhythm::sigCloseDialog, dialog->closeButton, &CloseButton::sigActivated);
    auto* animation = new QPropertyAnimation(dialog,"windowOpacity");
    animation->setDuration(100);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    dialog->exec();
}
