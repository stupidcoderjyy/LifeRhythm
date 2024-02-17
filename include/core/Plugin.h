//
// Created by stupid_coder_jyy on 2024/2/17.
//

#ifndef LIFERHYTHM_PLUGIN_H
#define LIFERHYTHM_PLUGIN_H

#include "Error.h"
#include "ErrorHandler.h"
#include "Constants.h"
#include "windows.h"
#include <QVector>
#include <QMap>
#include <QObject>
#include <QRegExp>

LR_BEGIN

#define PLUGIN_EXPORT extern "C" __declspec(dllexport)

#define PLUGIN_API_VERSION(v) PLUGIN_EXPORT const char* apiVersion(){ return v; }
#define PLUGIN_ID(v) PLUGIN_EXPORT const char* pluginId(){ return v; }
#define PLUGIN_NAME(v) PLUGIN_EXPORT const char* pluginName(){ return v; }
#define PLUGIN_DEPENDENCIES(v) PLUGIN_EXPORT const char* pluginDependencies(){ return v; }
#define PLUGIN_ENTRY_POINT PLUGIN_EXPORT void loadPlugin()
#define PLUGIN_CLOSE PLUGIN_EXPORT void unloadPlugin()

typedef const char* (*FuncStrGetter)();
typedef void (*FuncEntryPoint)();
typedef void (*FuncClose)();

class Plugin{
    friend class PluginManager;
private:
    HMODULE dll;
    Version apiVersion;
    QString id;
    QString name;
    QVector<Plugin*> dependencies{};
    FuncEntryPoint entryPoint;
    FuncClose closer;
    bool loaded = false;
    int regId;
public:
    void load();
    void unload();
    QString getId() const;
    QString getName() const;
    Version getApiVersion() const;
    virtual ~Plugin();
private:
    explicit Plugin(HMODULE dll, Version apiVersion, QString id,
                    QString name, FuncEntryPoint f3, FuncClose f4);
    void addDependency(Plugin* plugin);
};

class PluginManager : public QObject {
    Q_OBJECT
private:
    QStringList searchPaths{};
    ErrorHandler<Error>* errHandler{};
    QMap<QString, Plugin*> plugins{};
    QRegExp idRule;
public:
    PluginManager();
    void setErrorHandler(ErrorHandler<Error>* handler);
    void addSearchPath(const QString& path);
    void preInit();
    void mainInit();
signals:
    void sigPluginPreInitFinished();
    void sigPluginMainInit(Plugin* plugin);
private:
    void loadPlugins();
    void initPlugins();
    Plugin* loadDll(const QString& path);
    void parseDependencies(Plugin* plugin);
    static Version checkVersion(const QString& name, const QString& ver);
};

LR_END

#endif //LIFERHYTHM_PLUGIN_H
