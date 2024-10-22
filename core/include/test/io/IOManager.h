//
// Created by JYY on 24-10-11.
//

#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <QMap>
#include <qmutex.h>
#include "Identifier.h"
#include "Plugin.h"

class IByteWriter;
class IByteReader;

class CORE_API AbstractLoader : public QObject {
    Q_OBJECT
    friend class IOManager;
protected:
    bool loaded;
    bool dirty;
public:
    inline void setOnLoaded(const std::function<void()>& onLoaded) const;
    inline bool isLoaded() const;
    inline bool isDirty() const;
    inline void setDirty(bool d);
protected:
    virtual void load(IByteReader* in) = 0;
    virtual void clear() = 0;
    virtual void save(IByteWriter* out) = 0;
private:
    inline void load0(IByteReader* in);
    inline void save0(IByteWriter* out);
signals:
    void sigLoadFinished();
};

void AbstractLoader::setOnLoaded(const std::function<void()>& onLoaded) const {
    connect(this, &AbstractLoader::sigLoadFinished, onLoaded);
}

bool AbstractLoader::isLoaded() const {
    return loaded;
}

bool AbstractLoader::isDirty() const {
    return dirty;
}

void AbstractLoader::setDirty(bool d) {
    dirty = d;
}

inline void AbstractLoader::load0(IByteReader *in) {
    load(in);
    loaded = true;
}

inline void AbstractLoader::save0(IByteWriter *out) {
    save(out);
    dirty = false;
}

class CORE_API IOManager {
private:
    QMap<Identifier, AbstractLoader*> elements;
    QMutex mutex;
public:
    void registerSerializable(const Identifier& loc, AbstractLoader* serializable);
    void unregisterSerializable(const Identifier& loc);
    void globalLoad();
    void globalClear();
    void globalSave();
};

#endif //IOMANAGER_H
