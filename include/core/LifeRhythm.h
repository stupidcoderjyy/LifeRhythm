//
// Created by stupid_coder_jyy on 2024/2/9.
//

#ifndef LIFERHYTHM_LIFERHYTHM_H
#define LIFERHYTHM_LIFERHYTHM_H

#include <QString>
#include <QObject>
#include <QApplication>
#include "Plugin.h"

LR_BEGIN

class LifeRhythm;

class PluginErrorHandler : public ErrorHandler<Error> {
private:
    LifeRhythm* lr;
public:
    explicit PluginErrorHandler(LifeRhythm *lr);
    void onErrorCaught(Error &err) override;
};

class MainFrame;

class LifeRhythm : public QObject{
    Q_OBJECT
public:
    const static QString NAME;
    const static Version API_VERSION;
private:
    Config config;
    QApplication app;
    static LifeRhythm* lr;
    MainFrame* mainFrame{};
    PluginManager pluginManager;
    PluginErrorHandler pluginErrorHandler;
public:
    static LifeRhythm* get();
    static void generateTitledDialog(const QString& title, QWidget* content);
    void onPostInit(std::function<void()> handler, Qt::ConnectionType type = Qt::DirectConnection);
    void onMainInit(std::function<void()> handler, Qt::ConnectionType type = Qt::DirectConnection);
    LifeRhythm(int argc, char *argv[]);
    int launch();
    void setConfig(const Config& cfg);
    const Config& getConfig();
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
private slots:
    void prepareScreen();
};

LR_END

#endif //LIFERHYTHM_LIFERHYTHM_H
