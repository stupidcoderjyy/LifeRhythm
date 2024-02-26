//
// Created by stupid_coder_jyy on 2024/2/22.
//

#ifndef LIFERHYTHM_DATALOADER_H
#define LIFERHYTHM_DATALOADER_H

#include <utility>

#include "Identifier.h"
#include "StreamByteReader.h"
#include "StreamByteWriter.h"
#include "Serializable.h"
#include "Error.h"
#include "IOUtil.h"
#include "models/WidgetData.h"

template<class T>
class DataLoader {
protected:
    //存档的位置。如"log:today"，指代文件夹"<存档根目录>/log/today"
    QString destPath;
    bool loaded{};
public:
    DataLoader(const QString& savePath, const QString& childPath) {
        destPath = concatPath(savePath, childPath);
    }
    virtual void load() = 0;
    virtual void unload() = 0;
    virtual void save() = 0;
protected:
    virtual T* emptyElement() noexcept {
        return new T();
    }

    virtual void deleteElement(T* p) noexcept {
        delete p;
    }
};

template<class T>
class ListDataLoader : public DataLoader<T> {
protected:
    QString filePath;
    QVector<T*> es{};
public:
    ListDataLoader(const QString& savePath, const QString& childPath, const QString& fileName);
    explicit ListDataLoader(const QString& path, const QString& fileName);
    void load() override;
    void unload() override;
    void save() override;
    QVector<T*>& getData();
};

template<class T>
ListDataLoader<T>::ListDataLoader(const QString &savePath, const QString &childPath, const QString &fileName):
        DataLoader<T>(savePath, childPath) {
    filePath = concatPath(DataLoader<T>::destPath, fileName);
}

template<class T>
ListDataLoader<T>::ListDataLoader(const QString &path, const QString &fileName):
        ListDataLoader<T>("", path, fileName) {
}

template<class T>
void ListDataLoader<T>::load() {
    if (DataLoader<T>::loaded) {
        return;
    }
    StreamByteReader* reader;
    try {
        reader = new StreamByteReader(filePath);
    } catch (Error& e) {
        return;
    }
    int count = reader->readInt();
    for (int i = 0 ; i < count ; i ++) {
        T* p = DataLoader<T>::emptyElement();
        p->fromBytes(reader);
        es << p;
    }
    delete reader;
    DataLoader<T>::loaded = true;
}

template<class T>
void ListDataLoader<T>::unload() {
    if (!DataLoader<T>::loaded) {
        return;
    }
    for (T* e : es) {
        DataLoader<T>::deleteElement(e);
    }
    es.clear();
    DataLoader<T>::loaded = false;
}

template<class T>
void ListDataLoader<T>::save() {
    auto* writer = new StreamByteWriter(filePath);
    writer->writeInt(es.length());
    for (T* e : es) {
        e->toBytes(writer);
    }
    delete writer;
}

template<class T>
QVector<T *>& ListDataLoader<T>::getData() {
    return es;
}

#endif //LIFERHYTHM_DATALOADER_H
