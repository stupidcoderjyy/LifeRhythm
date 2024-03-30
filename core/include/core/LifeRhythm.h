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

BEGIN_NAMESPACE(lr)

class MainFrame;
class TabWidget;
class Module;

class LifeRhythm : public QObject{
    Q_OBJECT
public:
    const static QString NAME;
private:
    static LifeRhythm* lr;
    Config config;
    QApplication app;
    MainFrame* mainFrame;
    QMap<QString, Module*> modules;
public:
    static LifeRhythm* get();
    static void generateTitledDialog(const QString& title, QWidget* content);
    static void insertTab(const QString& title, TabWidget* tab, const Identifier& icon = "lr:icon_30");
    LifeRhythm(int argc, char *argv[]);
    void onPreInit(std::function<void()> handler);
    void onPostInit(std::function<void()> handler, Qt::ConnectionType type = Qt::DirectConnection);
    void onMainInit(std::function<void()> handler, Qt::ConnectionType type = Qt::DirectConnection);
    int launch();
    void setConfig(const Config& cfg);
    const Config& getConfig();
    void registerModule(Module* m);
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
};

END_NAMESPACE

#endif //LIFERHYTHM_LIFERHYTHM_H
