//
// Created by stupid_coder_jyy on 2024/2/9.
//

#ifndef LIFERHYTHM_LIFERHYTHM_H
#define LIFERHYTHM_LIFERHYTHM_H

#include <QString>
#include <QObject>
#include <QApplication>
#include "Namespaces.h"
#include "Identifier.h"
#include "Config.h"
#include "IOManager.h"
#include "TickHandler.h"

BEGIN_NAMESPACE(lr)
    class MainFrame;
class TabWidget;
class Module;

class CORE_API LifeRhythm final : public QObject {
    Q_OBJECT
public:
    const static QString NAME;
private:
    static LifeRhythm* lr;
    Config config;
    QApplication app;
    MainFrame* mainFrame;
    QMap<QString, Module*> modules;
    IOManager ioManager;
    TickHandler tickHandler;
public:
    static LifeRhythm* get();
    static void generateTitledDialog(const QString& title, QWidget* content);
    static void insertTab(const QString& title, TabWidget* tab, const Identifier& icon = LOC("lr:icon_30"));
    LifeRhythm(int argc, char *argv[]);
    void onPreInit(std::function<void()> handler);
    void onPostInit(std::function<void()> handler, Qt::ConnectionType type = Qt::DirectConnection);
    void onMainInit(std::function<void()> handler, Qt::ConnectionType type = Qt::DirectConnection);
    int launch();
    void setConfig(const Config& cfg);
    const Config& getConfig();
    void registerModule(Module* m);
    IOManager& getIOManager();
    TickHandler& getTickHandler();
signals:
    void sigPreInit();
    void sigMainInit();
    void sigPostInit();
    void sigCloseDialog();
private:
    void launch0();
    void preInit();
    void mainInit();
    void postInit();
    static void loadPlugins() ;
    static void exit();
};

END_NAMESPACE

#endif //LIFERHYTHM_LIFERHYTHM_H
