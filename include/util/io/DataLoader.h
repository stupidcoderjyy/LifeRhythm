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
#include "WidgetData.h"

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
class ListedDataLoader : public DataLoader<T>, public IListModel{
private:
    QString filePath;
    QVector<T*> es{};
public:
    ListedDataLoader(const QString& savePath, const QString& childPath, const QString& fileName);
    explicit ListedDataLoader(const QString& path, const QString& fileName);
    void load() override;
    void unload() override;
    void save() override;
    int length() const override;
    WidgetData *at(int idx) noexcept override;
    void append(WidgetData *data) override;
    void insert(int idx, WidgetData *data) override;
    WidgetData* remove(int idx) override;
    QVector<T*>& getData();
};

template<class T>
ListedDataLoader<T>::ListedDataLoader(const QString &savePath, const QString &childPath, const QString &fileName):
        DataLoader<T>(savePath, childPath) {
    filePath = concatPath(DataLoader<T>::destPath, fileName);
}

template<class T>
ListedDataLoader<T>::ListedDataLoader(const QString &path, const QString &fileName):
        ListedDataLoader<T>("", path, fileName) {
}

template<class T>
void ListedDataLoader<T>::load() {
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
void ListedDataLoader<T>::unload() {
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
void ListedDataLoader<T>::save() {
    auto* writer = new StreamByteWriter(filePath);
    writer->writeInt(es.length());
    for (T* e : es) {
        e->toBytes(writer);
    }
    delete writer;
}

template<class T>
int ListedDataLoader<T>::length() const {
    return es.length();
}

template<class T>
WidgetData *ListedDataLoader<T>::at(int idx) noexcept {
    return idx < es.length() ? es.at(idx) : nullptr;
}

template<class T>
void ListedDataLoader<T>::append(WidgetData *data) {
    int old = es.length();
    es.append(data);
    emit sigDataChanged(old, old);
}

template<class T>
void ListedDataLoader<T>::insert(int idx, WidgetData *data) {
    es.insert(idx, data);
    emit sigDataChanged(idx, es.length() - 1);
}

template<class T>
WidgetData* ListedDataLoader<T>::remove(int idx) {
    auto* d = es.at(idx);
    es.remove(idx);
    emit sigDataChanged(idx, idx < es.length() ? es.length() - 1 : idx);
    return d;
}

template<class T>
QVector<T *>& ListedDataLoader<T>::getData() {
    return es;
}

#endif //LIFERHYTHM_DATALOADER_H
