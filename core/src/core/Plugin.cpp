//
// Created by stupid_coder_jyy on 2024/2/17.
//

#include "Error.h"
#include "Plugin.h"
#include "StringInput.h"
#include "windows.h"
#include "LifeRhythm.h"
#include <utility>
#include <QDir>

USING_LR

int pluginCount = 0;

Plugin::Plugin(HMODULE dll, Version apiVersion, QString id, QString name, FuncEntryPoint f3, FuncClose f4):
    dll(dll), apiVersion(std::move(apiVersion)), id(std::move(id)),
    name(std::move(name)), entryPoint(f3), closer(f4), regId(pluginCount++){
}

void Plugin::load() {
    if (loaded) {
        return;
    }
    entryPoint();
    loaded = true;
}

void Plugin::unload() {
    closer();
}

QString Plugin::getId() const {
    return id;
}

QString Plugin::getName() const {
    return name;
}

Version Plugin::getApiVersion() const {
    return apiVersion;
}

Plugin::~Plugin() {
    FreeLibrary(dll);
}

void Plugin::addDependency(Plugin *plugin) {
    dependencies << plugin;
}

PluginManager::PluginManager():QObject() {
    idRule = QRegExp("[a-zA-Z]\\w*");
}

void PluginManager::setErrorHandler(ErrorHandler<Error> *handler) {
    errHandler = handler;
}

void PluginManager::preInit() {
    try {
        loadPlugins();
        emit sigPluginPreInitFinished();
    } catch (Error& err) {
        errHandler->onErrorCaught(err);
    }
}

void PluginManager::mainInit() {
    try {
        initPlugins();
    } catch (Error& err) {
        errHandler->onErrorCaught(err);
    }
}

void PluginManager::loadPlugins() {
    for (auto& path : searchPaths) {
        auto dlls = QDir(path).entryInfoList({"*.dll"}, QDir::Files);
        for (auto& dllFile : dlls) {
            loadDll(dllFile.absoluteFilePath().replace('/', '\\'));
        }
    }
}

template<class T> T loadFunc(HMODULE dll, const QString& fileName, const char* funcName) {
    auto* f = (T)GetProcAddress(dll, funcName);
    if (!f) {
        throw Error(fileName, "missing func '" + QString(funcName) + "', err " + QString::number(GetLastError()));
    }
    return f;
}

Plugin* PluginManager::loadDll(const QString &path) {
    HMODULE dll = LoadLibraryA(path.toStdString().c_str());
    if (!dll) {
        throw Error(path, "failed to load dll file: " + QString::number(GetLastError()));
    }
    auto* f0 = loadFunc<FuncStrGetter>(dll, path, "apiVersion");
    Version version = checkVersion(path, f0());
    auto* f1 = loadFunc<FuncStrGetter>(dll, path, "pluginId");
    QString id = f1();
    if (plugins.contains(id)) {
        throw Error(path, "plugin id occupied: '" + id + "'");
    }
    if (!idRule.exactMatch(id)) {
        throw Error(path, "plugin id doesn't match [a-zA-Z]\\w*: '" + id + "'");
    }
    auto* f2 = loadFunc<FuncStrGetter>(dll, path, "pluginName");
    auto* f3 = loadFunc<FuncEntryPoint>(dll, path, "loadPlugin");
    auto* f4 = loadFunc<FuncClose>(dll, path, "unloadPlugin");
    auto* plugin = new Plugin(dll, version, id, f2(), f3, f4);
    plugins.insert(plugin->id, plugin);
    return plugin;
}

Version PluginManager::checkVersion(const QString& path, const QString& ver) {
    int i = ver.indexOf('.');
    bool b1, b2 = true;
    int major = ver.mid(0, i).toInt(&b1);
    int minor = i < 0 ? 0 : ver.mid(i + 1, ver.length() - i).toInt(&b2);
    if (!b1 || !b2) {
        throw Error(path, "invalid version: " + ver);
    }
    Version version(major, minor);
    if (version == LifeRhythm::API_VERSION) {
        return version;
    }
    throw Error(path, "mismatched api version, "
            "required: '" + ver + "', "
            "provided: '" + LifeRhythm::API_VERSION.toString() + "'");
}

void PluginManager::initPlugins() {
    if (plugins.empty()) {
        return;
    }
    for (auto* plugin : plugins) {
        parseDependencies(plugin);
    }
    bool visited[pluginCount];
    memset(visited, false, pluginCount);
    QVector<Plugin*> waitStack;
    waitStack << plugins.first();
    while (!waitStack.empty()) {
        Plugin* target = waitStack.last();
        if (target->dependencies.empty() || visited[target->regId]) {
            target->load();
            emit sigPluginMainInit(target);
            waitStack.removeLast();
            continue;
        }
        visited[target->regId] = true;
        for (Plugin* child : target->dependencies) {
            if (child->loaded) {
                continue;
            }
            if (visited[child->regId]) {
                throw Error(target->id, "circular dependency");
            }
            waitStack << child;
        }
    }
}

void PluginManager::parseDependencies(Plugin* plugin) {
    auto f = (FuncStrGetter)GetProcAddress(plugin->dll, "pluginDependencies");
    if (!f) {
        return;
    }
    StringInput in(f());
    while (in.available()) {
        in.mark();
        in.approach(',');
        in.mark();
        QString parentId = in.capture();
        if (in.available()) {
            in.read();
        }
        Plugin* parent = plugins.value(parentId);
        if (!parent) {
            throw Error(plugin->getId(), "missing dependencies '" + parentId + "'");
        }
        plugin->addDependency(parent);
    }
}

void PluginManager::addSearchPath(const QString &path) {
    searchPaths << path;
}
