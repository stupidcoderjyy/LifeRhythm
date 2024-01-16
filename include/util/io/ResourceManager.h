//
// Created by stupid_coder_jyy on 2024/1/14.
//

#ifndef LIFERHYTHM_RESOURCEMANAGER_H
#define LIFERHYTHM_RESOURCEMANAGER_H

#include "Identifier.h"
#include "QDir"
#include <QMap>
#include <utility>
#include "Error.h"
#include "MemUtil.h"
#include "PrintErrorHandler.h"
#include <QDebug>

class ResourceType{
private:
    QString pathPrefix;
    QString pathSuffix;
public:
    ResourceType(QString pathPrefix, QString fileSuffix);
    ResourceType(ResourceType&& o) noexcept;
    Identifier buildFilePath(const Identifier& loc) const;
    Identifier buildDirPath(const Identifier& loc) const;
    const QString &getPathPrefix() const;
    const QString &getPathSuffix() const;
};

template<class T>
class ResourceManager{
protected:
    QMap<Identifier, T*> data;
    QStringList nameFilters;
    ResourceType type;
public:
    explicit ResourceManager(ResourceType type, QStringList nameFilters):
            nameFilters(std::move(nameFilters)),
            type(std::move(type)){
    }
    T* get(const Identifier& loc) {
        return data.value(loc, nullptr);
    }
    bool exists(const Identifier& loc) {
        return data.contains(loc);
    }
    void init(const QString& _namespace) {
        _init(_namespace, "");
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
    virtual T* load(const Identifier& loc) = 0;
    virtual void loadFailure(std::exception& e) noexcept {
    };
private:
    void _init(const QString& _namespace, const QString& childPath) {
        QString dirPath = Identifier::absolutePath(_namespace, type.getPathPrefix(), childPath);
        QFileInfoList files = QDir(dirPath).entryInfoList(nameFilters);
        for (auto& info : files) {
            QString extendedPath = childPath.isEmpty() ?
                    info.baseName() :
                    childPath + "/" + info.baseName();
            if (info.isFile()) {
                QString filePath = dirPath + "/" + info.fileName();
                Identifier fileLoc = Identifier(_namespace, extendedPath);
                try {
                    T* val = load(fileLoc);
                    data.insert(fileLoc, val);
                } catch (std::exception& e) {
                    Error err("ResourceManager::_init", "failed to load resource '" + filePath + "'");
                    PrintErrorHandler().onErrorCaught(err);
                    loadFailure(e);
                }
            } else {
                _init(_namespace, extendedPath);
            }
        }
    }
};

#endif //LIFERHYTHM_RESOURCEMANAGER_H
