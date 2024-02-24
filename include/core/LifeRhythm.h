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
    Config config;
private:
    QApplication app;
    static LifeRhythm* lr;
    MainFrame* mainFrame{};
    PluginManager pluginManager;
    PluginErrorHandler pluginErrorHandler;
public:
    LifeRhythm(int argc, char *argv[]);
    static LifeRhythm* get();
    int launch();
    static void generateTitledDialog(const QString& title, QWidget* content);
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

LR_END

#endif //LIFERHYTHM_LIFERHYTHM_H
