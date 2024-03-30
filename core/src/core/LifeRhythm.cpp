//
// Created by stupid_coder_jyy on 2024/2/9.
//

#include "LifeRhythm.h"
#include "MainFrame.h"
#include "TabBar.h"
#include "IOUtil.h"
#include "RcManagers.h"
#include "TitledDialog.h"
#include "ColorSelector.h"
#include "WidgetDataStorage.h"
#include <QtConcurrent>
#include <QApplication>
#include <utility>

USING_LR

void PluginErrorHandler::onErrorCaught(Error &err) {
    qDebug() << ("failed to load plugin: " + err.why + ", at: " + err.where).toUtf8().data();
}

PluginErrorHandler::PluginErrorHandler(LifeRhythm *lr):lr(lr) {
}

const QString LifeRhythm::NAME = "LifeRhythm-dev";
const Version LifeRhythm::API_VERSION(1, 0);

LifeRhythm* LifeRhythm::lr{};

LifeRhythm *LifeRhythm::get() {
    return lr;
}

int LifeRhythm::launch() {
    launch0();
    return QApplication::exec();
}

void LifeRhythm::setConfig(const Config &cfg) {
    config = cfg;
}

void LifeRhythm::launch0() {
    preInit();
    emit sigPreInit();
    mainInit();
    emit sigMainInit();
    postInit();
    prepareScreen();
    emit sigPostInit();
//    auto* watcher = new QFutureWatcher<void>(this);
//    QFutureInterface<void> futureInterface;
//    futureInterface.future();
//    QFuture<void> future = QtConcurrent::run([this]() {
//        mainInit();
//    });
//    watcher->setFuture(future);
//
//    connect(watcher, &QFutureWatcher<void>::finished, this, [this](){
//        emit sigMainInit();
//    });
}

void LifeRhythm::preInit() {
    config.froze();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    Styles::initStyles();
    pluginManager.preInit();
}

void LifeRhythm::mainInit() {
    pluginManager.mainInit();
    ImageStorage::init();
    WidgetFactoryStorage::init();
    WidgetFactory::mainInit();
    auto* f = WidgetFactoryStorage::get("lr:widget_tabcard");
    regClazz(f, TabCard);
    regClazz(f, CloseButton);
    f = WidgetFactoryStorage::get("lr:widget_titleddialog");
    regClazz(f, TitledDialog);
    regClazz(f, DialogCloseButton);
    regClazz(f, TopWidget);
    f = WidgetFactoryStorage::get("lr:widget_mainframe");
    regClazz(f, MainFrame);
    regClazz(f, TabBar);
    f = WidgetFactoryStorage::get("lr:item_defaultcolors");
    regClazz(f, DefaultColorsListItem);
    auto* m0 = new SelectableListData;
    for (auto& c : Color::defaultColors) {
        m0->append(c);
    }
    WidgetDataStorage::add("lr:default_colors", m0);
}

void LifeRhythm::postInit() {
    WidgetFactoryStorage::parseAll();
}

void LifeRhythm::prepareScreen() {
    if (config.mode == Config::Test) {
        return;
    }
    mainFrame = static_cast<MainFrame*>(WidgetFactoryStorage::get("lr:widget_mainframe")->apply());
}

LifeRhythm::LifeRhythm(int argc, char *argv[]):QObject(),
        pluginErrorHandler(this), pluginManager(), app(argc, argv), config() {
    lr = this;
    pluginManager.setErrorHandler(&pluginErrorHandler);
    pluginManager.addSearchPath("testplugins");
//    connect(this, &LifeRhythm::sigPostInit, this, &LifeRhythm::prepareScreen, Qt::QueuedConnection);
}

void LifeRhythm::generateTitledDialog(const QString &title, QWidget *content) {
    auto* dialog = static_cast<TitledDialog*>(WidgetFactoryStorage::get("lr:widget_titleddialog")->apply());
    dialog->setContent(title, content);
    connect(lr, &LifeRhythm::sigCloseDialog, dialog->closeButton, &CloseButton::sigActivated);
    auto* animation = new QPropertyAnimation(dialog,"windowOpacity");
    animation->setDuration(100);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    dialog->exec();
}

void LifeRhythm::insertTab(const QString& title, TabWidget *tab, const Identifier &icon) {
    lr->mainFrame->tabBar->insertTab(title, tab, icon);
}

void LifeRhythm::insertTab(const QString &title, const Identifier &factory, const Identifier &icon) {
    auto* f = WidgetFactoryStorage::get(factory);
    if (!f) {
        throwInFunc("invalid factory: " + factory.getPath());
    }
    auto* w = dynamic_cast<TabWidget*>(f->apply());
    if (!w) {
        throwInFunc("requires instance of TabWidget, factory: " + factory.getPath());
    }
    insertTab(title, w, icon);
}

void LifeRhythm::onPreInit(std::function<void()> handler) {
    connect(this, &LifeRhythm::sigPreInit, this, std::move(handler));
}

void LifeRhythm::onPostInit(std::function<void()> handler, Qt::ConnectionType type) {
    connect(this, &LifeRhythm::sigPostInit, this, std::move(handler), type);
}

void LifeRhythm::onMainInit(std::function<void()> handler, Qt::ConnectionType type) {
    connect(this, &LifeRhythm::sigMainInit, this, std::move(handler), type);
}

const Config &LifeRhythm::getConfig() {
    return config;
}
