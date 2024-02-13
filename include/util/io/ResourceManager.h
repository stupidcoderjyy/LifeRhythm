//
// Created by stupid_coder_jyy on 2024/1/14.
//

#ifndef LIFERHYTHM_RESOURCEMANAGER_H
#define LIFERHYTHM_RESOURCEMANAGER_H

#include "Identifier.h"
#include "Error.h"
#include "MemUtil.h"
#include "IOUtil.h"
#include "PrintErrorHandler.h"
#include <QDebug>
#include <QDir>
#include <QMap>
#include <utility>

class ResourceType{
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
class ResourceManager{
protected:
    QMap<Identifier, T*> data;
    QStringList nameFilters;
    ResourceType type;
    QString rootPath;
public:
    explicit ResourceManager(QString rootPath, ResourceType type):
            nameFilters({'*' + type.getFileType()}),
            type(std::move(type)),
            rootPath(std::move(rootPath)){
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
            _init(ns.baseName(), rootPath + "/" + ns.baseName() + "/" + type.getTypeDir(), "");
        }
    }

    void unload(const QString& _namespace) {
        auto it = data.begin();
        QVector<Identifier> targets{};
        while (it != data.end()) {
            if (it.key().getNamespace() == _namespace) {
                targets << it.key();
                delete it.value();
            }
            it++;
        }
        for (auto& loc : targets) {
            data.remove(loc);
        }
    }

    virtual ~ResourceManager() {
        DELETE_MAP(data)
    }
protected:
    virtual T* load(const Identifier& loc, const QString& absolutePath) = 0;
    virtual void loadFailure(std::exception& e) noexcept {
    };
private:
    void _init(const QString& ns, const QString& basePath, const QString& childPath) {
        QString dirPath = concatPath(basePath, childPath);
        QFileInfoList files = QDir(dirPath).entryInfoList(nameFilters, QDir::AllDirs | QDir::Files);
        for (auto& info : files) {
            QString extendedPath = childPath.isEmpty() ?
                    info.baseName() :
                    childPath + "/" + info.baseName();
            if (info.isFile()) {
                QString filePath = dirPath + "/" + info.fileName();
                Identifier fileLoc = Identifier(ns, extendedPath);
                try {
                    T* val = load(fileLoc, filePath);
                    data.insert(fileLoc, val);
                } catch (std::exception& e) {
                    Error err("ResourceManager::_init", "failed to load resource '" + filePath + "'");
                    PrintErrorHandler().onErrorCaught(err);
                    loadFailure(e);
                }
            } else {
                _init(ns, dirPath, extendedPath);
            }
        }
    }
};

template<class T>
class BuiltInResourceManager : public ResourceManager<T> {
public:
    explicit BuiltInResourceManager(const ResourceType &type):ResourceManager<T>(":/assets", type) {
    }
};

#endif //LIFERHYTHM_RESOURCEMANAGER_H
