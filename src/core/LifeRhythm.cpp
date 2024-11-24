//
// Created by stupid_coder_jyy on 2024/2/9.
//

#include "LifeRhythm.h"
#include "MainFrame.h"
#include "TabBar.h"
#include "RcManagers.h"
#include "TitledDialog.h"
#include "ColorSelector.h"
#include "WidgetDataStorage.h"
#include "Module.h"
#include <QtConcurrent>
#include <QApplication>
#include <utility>
#include <windows.h>

USING_NAMESPACE(lr)

LifeRhythm* LifeRhythm::lr{};

LifeRhythm *LifeRhythm::get() {
    return lr;
}

void LifeRhythm::generateTitledDialog(const QString &title, QWidget *content) {
    auto* dialog = WidgetFactoryStorage::get("lr:widget_titleddialog")->applyAndCast<TitledDialog>();
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

LifeRhythm::LifeRhythm(int argc, char *argv[]): app(argc, argv), mainFrame(), tickHandler(5000) {
    lr = this;
}

void LifeRhythm::onPreInit(std::function<void()> handler) {
    connect(this, &LifeRhythm::sigPreInit, this, std::move(handler));
}

void LifeRhythm::onPostInit(std::function<void()> handler, const Qt::ConnectionType type) {
    connect(this, &LifeRhythm::sigPostInit, this, std::move(handler), type);
}

void LifeRhythm::onMainInit(std::function<void()> handler, const Qt::ConnectionType type) {
    connect(this, &LifeRhythm::sigMainInit, this, std::move(handler), type);
}

int LifeRhythm::launch() {
    launch0();
    return QApplication::exec();
}

void LifeRhythm::setConfig(const Config &cfg) {
    config = cfg;
}

const Config &LifeRhythm::getConfig() {
    return config;
}

void LifeRhythm::registerModule(Module *m) {
    if (config.frozen) {
        throwInFunc("frozen");
    }
    if (!m) {
        throwInFunc("null module");
    }
    if (modules.contains(m->id)) {
        throwInFunc("id '" + m->id + "' is already registered");
    }
    modules.insert(m->id, m);
}

IOManager& LifeRhythm::getIOManager() {
    return ioManager;
}

TickHandler& LifeRhythm::getTickHandler() {
    return tickHandler;
}

MessageHandler& LifeRhythm::getMessageHandler() {
    return messageHandler;
}

void LifeRhythm::launch0() {
    preInit();
    emit sigPreInit();
    mainInit();
    emit sigMainInit();
    postInit();
    emit sigPostInit();
}

void LifeRhythm::preInit() {
    config.froze();
    // atexit(exit);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    Styles::initStyles();
    if (config.mode == Config::Test) {
        return;
    }
    loadPlugins();
    for (auto* m : modules) {
        m->preInit();
    }
}

void LifeRhythm::mainInit() {
    ImageStorage::init();
    WidgetFactoryStorage::init();
    WidgetFactory::mainInit();
    // auto* f = WidgetFactoryStorage::get("lr:widget_tabcard");
    // regClazz(f, TabCard);
    // regClazz(f, CloseButton);
    // f = WidgetFactoryStorage::get("lr:widget_titleddialog");
    // regClazz(f, TitledDialog);
    // regClazz(f, DialogCloseButton);
    // regClazz(f, TopWidget);
    // f = WidgetFactoryStorage::get("lr:widget_mainframe");
    // regClazz(f, MainFrame);
    // regClazz(f, TabBar);
    // f = WidgetFactoryStorage::get("lr:item_defaultcolors");
    // regClazz(f, DefaultColorsListItem);
    auto* m0 = new ListData;
    for (const auto& c : Color::defaultColors) {
        m0->append(c);
    }
    WidgetDataStorage::add(LOC("lr:default_colors"), m0);
    if (config.mode == Config::Test) {
        return;
    }
    ioManager.globalLoad();
    for (auto* m : modules) {
        m->mainInit();
    }
}

void LifeRhythm::postInit() {
    WidgetFactoryStorage::parseAll();
    // mainFrame = WidgetFactoryStorage::get("lr:widget_mainframe")->applyAndCast<MainFrame>();
    if (config.mode == Config::Test) {
        return;
    }
    for (auto* m : modules) {
        m->postInit();
    }
    tickHandler.start();
}

void LifeRhythm::loadPlugins() {
    auto files = QDir("").entryList({"*.dll"}, QDir::Files);
    for (const auto& file : files) {
        if (!file.startsWith("lr-")) {
            continue;
        }
        qDebug() << "loading plugin: " << file;
        QLibrary lib(file);
        if (!lib.load()) {
            qDebug() << lib.errorString();
            continue;
        }
        auto entrypoint = lib.resolve("pluginEntryPoint");
        if (!entrypoint) {
            qDebug() << "FAILED: missing entrypoint";
            continue;
        }
        entrypoint();
        qDebug() << "SUCCESS\n";
    }
}

void LifeRhythm::exit() {
    lr->ioManager.globalSave();
}
