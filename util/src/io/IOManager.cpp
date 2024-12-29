//
// Created by JYY on 24-10-11.
//

#include "IOManager.h"
#include "AsyncTask.h"
#include "Error.h"
#include "IOUtil.h"
#include "StreamByteReader.h"
#include "StreamByteWriter.h"

USING_LR

void AbstractLoader::load(IByteReader *in) {
}

void AbstractLoader::clear() {
}

void AbstractLoader::save(IByteWriter *out) {
}

void IOManager::registerSerializable(const Identifier &loc, AbstractLoader *serializable) {
    mutex.lock();
    elements.insert(loc, serializable);
    mutex.unlock();
}

void IOManager::unregisterSerializable(const Identifier &loc) {
    mutex.lock();
    elements.remove(loc);
    mutex.unlock();
}

void IOManager::globalLoad(const QString &p) {
    auto *task = AsyncTask::create();
    task->setTask([this, p] {
        mutex.lock();
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            auto path = concatPath(p, it.key().toFullPath());
            auto* handler = it.value();
            if (handler->loaded) {
                continue;
            }
            StreamByteReader* reader = nullptr;
            try {
                reader = new StreamByteReader(path);
                handler->load(reader);
                handler->loaded = true;
                emit handler->sigLoadFinished();
            } catch (Error&) {
            }
            delete reader;
        }
        mutex.unlock();
    });
    task->start();  //auto delete
}

void IOManager::globalClear(const QString &path) {
    for (const auto e : elements) {
        e->clear();
        e->loaded = false;
    }
}

void IOManager::globalSave(const QString &p) {
    auto *task = AsyncTask::create();
    task->setTask([this, p] {
        mutex.lock();
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            auto path = concatPath(p, it.key().toFullPath());
            auto *handler = it.value();
            if (!handler->dirty) {
                continue;
            }
            StreamByteWriter *writer = nullptr;
            try {
                writer = new StreamByteWriter(path);
                handler->save(writer);
                handler->dirty = false;
            } catch (Error&) {
            }
            delete writer;
        }
        mutex.unlock();
    });
    task->start();  //auto delete
}
