//
// Created by stupid_coder_jyy on 2024/2/29.
//

#include "DataLoader.h"
#include "IOUtil.h"
#include "Error.h"
#include "StreamByteReader.h"
#include "StreamByteWriter.h"

DataLoader::DataLoader(const QString &savePath, const QString &childPath): QObject(), wData() {
    destPath = concatPath(savePath, childPath);
}

void DataLoader::load() {
    if (loaded) {
        return;
    }
    StreamByteReader* reader;
    try {
        reader = new StreamByteReader(destPath);
    } catch (Error& e) {
        return;
    }
    wData = emptyElement();
    wData->fromBytes(reader);
    loaded = true;
    emit sigLoaded();
}

void DataLoader::unload() {
    if (!loaded) {
        return;
    }
    delete wData;
    wData = nullptr;
    loaded = false;
    emit sigUnloaded();
}

void DataLoader::save() {
    if (!loaded) {
        return;
    }
    auto* writer = new StreamByteWriter(destPath);
    wData->toBytes(writer);
    delete writer;
}

void DataLoader::deleteElement(WidgetData *p) noexcept {
    delete p;
}
