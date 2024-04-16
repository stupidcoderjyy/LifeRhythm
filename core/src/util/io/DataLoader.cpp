//
// Created by stupid_coder_jyy on 2024/2/29.
//

#include "DataLoader.h"

#include <utility>
#include "IOUtil.h"
#include "Error.h"
#include "StreamByteReader.h"
#include "StreamByteWriter.h"
#include "ListData.h"

DataLoader::DataLoader(QString destPath): QObject(), wData(), destPath(std::move(destPath)) {
}

void DataLoader::load() {
    if (loaded) {
        return;
    }
    wData = createData();
    StreamByteReader* reader;
    try {
        reader = new StreamByteReader(destPath);
        wData->fromBytes(reader);
    } catch (Error& e) {
    }
    loaded = true;
    emit sigLoaded();
}

void DataLoader::unload() {
    if (!loaded) {
        return;
    }
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

WidgetData *DataLoader::getData() {
    return wData;
}

void DataLoader::destroyData(WidgetData *p) noexcept {
    delete p;
}

ListDataLoader::ListDataLoader(QString destPath): DataLoader(std::move(destPath)) {
}

void ListDataLoader::load() {
    if (loaded) {
        return;
    }
    auto* listData = dynamic_cast<ListData*>(createData());
    if (!listData) {
        throwInFunc("createData() must return ListData in ListDataLoader");
    }
    wData = listData;
    listData->beginEdit();
    StreamByteReader* reader;
    try {
        reader = new StreamByteReader(destPath);
        wData->fromBytes(reader);
    } catch (Error& e) {
    }
    delete reader;
    listData->endEdit();
    loaded = true;
    emit sigLoaded();
}
