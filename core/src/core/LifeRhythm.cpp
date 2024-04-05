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
#include "Calendar.h"
#include <QtConcurrent>
#include <QApplication>
#include <utility>

USING_NAMESPACE(lr)

LifeRhythm* LifeRhythm::lr{};

LifeRhythm *LifeRhythm::get() {
    return lr;
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

LifeRhythm::LifeRhythm(int argc, char *argv[]):QObject(),
        app(argc, argv), config(), mainFrame(), modules() {
    lr = this;
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
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    Styles::initStyles();
    if (config.mode == Config::Test) {
        return;
    }
    for (auto* m : modules) {
        m->preInit();
    }
}

void LifeRhythm::mainInit() {
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
    f = WidgetFactoryStorage::get("lr:widget_calendar");
    regClazz(f, WeekDayTitleDrawer);
    regClazz(f, CalendarContentDrawer);
    auto* m0 = new SelectableListData;
    for (auto& c : Color::defaultColors) {
        m0->append(c);
    }
    WidgetDataStorage::add("lr:default_colors", m0);
    if (config.mode == Config::Test) {
        return;
    }
    for (auto* m : modules) {
        m->mainInit();
    }
}

void LifeRhythm::postInit() {
    WidgetFactoryStorage::parseAll();
    mainFrame = static_cast<MainFrame*>(WidgetFactoryStorage::get("lr:widget_mainframe")->apply());
    if (config.mode == Config::Test) {
        return;
    }
    for (auto* m : modules) {
        m->postInit();
    }
}