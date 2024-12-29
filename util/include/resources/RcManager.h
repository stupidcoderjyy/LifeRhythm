//
// Created by stupid_coder_jyy on 2024/1/14.
//

#ifndef RCMANAGER_H
#define RCMANAGER_H

#include "Identifier.h"
#include "Error.h"
#include "Helpers.h"
#include "IOUtil.h"
#include "PrintErrorHandler.h"
#include <QDir>
#include <QMap>
#include <utility>
#include "RcLoader.h"
#include "Plugin.h"

BEGIN_LR

class CORE_API ResourceType{
private:
    QString typeDir;
    QString fileType;
public:
    ResourceType(QString typeDir, QString fileType);
    ResourceType(ResourceType&& o) noexcept;
    ResourceType(const ResourceType& o);
    QString buildPath(const Identifier& loc) const;
    const QString &getTypeDir() const;
    const QString &getFileType() const;
};

template<class T>
class CORE_API ResourceManager{
protected:
    QMap<Identifier, T*> data;
    QStringList nameFilters;
    ResourceType type;
    QString rootPath;
    RcLoader<T>* loader;
public:
    explicit ResourceManager(QString rootPath, ResourceType type, RcLoader<T>* loader):
            nameFilters({'*' + type.getFileType()}),
            type(std::move(type)),
            rootPath(std::move(rootPath)),
            loader(loader){
    }

    T* get(const Identifier& loc) {
        return data.value(loc, nullptr);
    }

    bool exists(const Identifier& loc) {
        return data.contains(loc);
    }

    void init() {
        QFileInfoList namespaces = QDir(rootPath).entryInfoList({}, QDir::AllDirs);
        for (auto& ns : namespaces) {
            if (ns.baseName().isEmpty()) {
                continue;
            }
            _init(ns.baseName(), rootPath + "/" + ns.baseName() + "/" + type.getTypeDir(), "");
        }
    }

    void unload(const QString& _namespace) {
        auto it = data.begin();
        QVector<Identifier> targets{};
        while (it != data.end()) {
            if (it.key().getNamespace() == _namespace) {
                targets << it.key();
                loader->deleteElement(it.value());
            }
            ++it;
        }
        for (auto& loc : targets) {
            data.remove(loc);
        }
    }

    QMap<Identifier, T*>& getData() {
        return data;
    }

    virtual ~ResourceManager() {
        for (T* p : data) {
            loader->deleteElement(p);
        }
    }
private:
    void _init(const QString& ns, const QString& basePath, const QString& childPath) {
        QString dirPath = concatPath(basePath, childPath);
        QFileInfoList files = QDir(dirPath).entryInfoList(nameFilters, QDir::AllDirs | QDir::Files);
        for (auto& info : files) {
            if (info.baseName().isEmpty()) {
                continue;
            }
            QString extendedPath = concatPath(childPath, info.baseName());
            if (info.isFile()) {
                QString filePath = dirPath + "/" + info.fileName();
                auto fileLoc = Identifier(ns, extendedPath);
                try {
                    T* val = loader->load(fileLoc, filePath);
                    data.insert(fileLoc, val);
                } catch (std::exception& e) {
                    Error err("ResourceManager::_init failed to load resource '" + filePath + "'");
                    PrintErrorHandler().onErrorCaught(err);
                    loader->onLoadFailed(e);
                }
            } else {
                _init(ns, dirPath, extendedPath);
            }
        }
    }
};

template<class T>
class CORE_API BuiltInResourceManager : public ResourceManager<T> {
public:
    explicit BuiltInResourceManager(const ResourceType &type, RcLoader<T>* loader):
            ResourceManager<T>("assets", type, loader) {
    }
};

#define STATIC_DEFINE(CLAZZ, TARGET) \
    private:               \
        static CLAZZ instance;     \
    public:                \
        static TARGET* get(const QString& loc) { return instance._get(LOC(loc)); } \
        static TARGET* get(const Identifier& loc) { return instance._get(loc); } \
        static bool exists(const QString& loc) { return instance._exists(LOC(loc)); } \
        static bool exists(const Identifier& loc) { return instance._exists(loc); } \
        static void addManager(ResourceManager<TARGET>* manager) { instance._addManager(manager); } \
        static void init() { instance._init(); } \
    protected: CLAZZ();

#define STATIC_INSTANCE(CLAZZ) CLAZZ CLAZZ::instance{};

template<class T>
class CORE_API MultiSourceResourceManager {
protected:
    QVector<ResourceManager<T>*> managers{};
protected:
    T* _get(const Identifier& loc) {
        T* res = nullptr;
        for (ResourceManager<T>*& child : managers) {
            res = child->get(loc);
            if (res) {
                break;
            }
        }
        return res;
    }

    bool _exists(const Identifier& loc) {
        for (ResourceManager<T>*& child : managers) {
            if (child->exists(loc)) {
                return true;
            }
        }
        return false;
    }

    void _addManager(ResourceManager<T>* manager) {
        managers << manager;
    }

    void _init() {
        for (ResourceManager<T>*& child : managers) {
            child->init();
        }
    }
};

END_LR

#endif //RCMANAGER_H
