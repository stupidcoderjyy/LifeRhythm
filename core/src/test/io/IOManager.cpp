//
// Created by JYY on 24-10-11.
//

#include "IOManager.h"

#include "Error.h"
#include "IOUtil.h"
#include "LifeRhythm.h"
#include "StreamByteReader.h"
#include "StreamByteWriter.h"

void IOManager::registerSerializable(const Identifier &loc, ISerializable *serializable) {
    elements.insert(loc, serializable);
}

void IOManager::load() {
    const auto cfg = lr::LifeRhythm::get()->getConfig();
    for (auto it = elements.begin(); it != elements.end(); ++it) {
        auto path = concatPath(cfg.getSavePath(), it.key().toFullPath());
        StreamByteReader* reader = nullptr;
        try {
            reader = new StreamByteReader(path);
            it.value()->load(reader);
        } catch (Error& e) {
        }
        delete reader;
    }
}

void IOManager::clear() {
    for (const auto e : elements) {
        e->clear();
    }
}

void IOManager::save() {
    const auto cfg = lr::LifeRhythm::get()->getConfig();
    for (auto it = elements.begin(); it != elements.end(); ++it) {
        auto path = concatPath(cfg.getSavePath(), it.key().toFullPath());
        StreamByteWriter* writer = nullptr;
        try {
            writer = new StreamByteWriter(path);
            it.value()->save(writer);
        } catch (Error& e) {
        }
        delete writer;
    }
}
