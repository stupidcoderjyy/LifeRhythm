//
// Created by stupid_coder_jyy on 2024/2/22.
//

#ifndef LIFERHYTHM_ELEMENTSMANAGER_H
#define LIFERHYTHM_ELEMENTSMANAGER_H

#include <utility>

#include "Identifier.h"
#include "StreamByteReader.h"
#include "StreamByteWriter.h"
#include "Serializable.h"
#include "Error.h"
#include "IOUtil.h"

//元素管理器，负责管理内存中元素和硬盘中存档的转化
template<class T>
class ElementsManager {
public:
    enum State{
        Empty,
        Loaded,
        Broken
    };
protected:
    //存档的位置。如"log:today"，指代文件夹"<存档根目录>/log/today"
    QString destPath;
    State state;
public:
    ElementsManager(const QString& savePath, const QString& childPath) {
        destPath = concatPath(savePath, childPath);
        state = Empty;
    }

    virtual void load() {
        state = Loaded;
    }

    //卸载加载的数据
    virtual void unload() {
        state = Empty;
    }

    virtual void save() {
    }

protected:
    virtual T* emptyElement() noexcept {
        return new T();
    }

    virtual void deleteElement(T* p) noexcept {
        delete p;
    }
};

template<class T>
class ListElementsManager : public ElementsManager<T>{
private:
    QString filePath;
    QVector<T*> es{};
public:
    ListElementsManager(const QString& savePath, const QString& childPath, const QString& fileName):
            ElementsManager<T>(savePath, childPath) {
        filePath = concatPath(ElementsManager<T>::destPath, fileName);
    }

    explicit ListElementsManager(const QString& path, const QString& fileName):ListElementsManager<T>("", path, fileName) {
    }

    void load() override {
        StreamByteReader* reader;
        try {
            reader = new StreamByteReader(filePath);
        } catch (Error& e) {
            return;
        }
        ElementsManager<T>::load();
        int count = reader->readInt();
        for (int i = 0 ; i < count ; i ++) {
            T* p = ElementsManager<T>::emptyElement();
            p->fromBytes(reader);
            es << p;
        }
        delete reader;
    }

    void unload() override {
        ElementsManager<T>::unload();
        for (T* e : es) {
            ElementsManager<T>::deleteElement(e);
        }
        es.clear();
    }

    void save() override {
        auto* writer = new StreamByteWriter(filePath);
        writer->writeInt(es.length());
        for (T* e : es) {
            e->toBytes(writer);
        }
        delete writer;
    }

    QVector<T*>& elements() {
        return es;
    }

    void add(T* e) {
        es << e;
    }
};

#endif //LIFERHYTHM_ELEMENTSMANAGER_H
