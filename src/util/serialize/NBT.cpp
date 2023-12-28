//
// Created by stupid_coder_jyy on 2023/12/28.
//
#include "NBT.h"
#include <QStringBuilder>
#include "Error.h"

NBT::NBT(): Data(COMPOUND) {
}

void NBT::serialize(IByteWriter *writer) {
    writer->writeByte(COMPOUND);
    writer->writeInt(data.size());
    for (auto it = data.begin() ; it != data.end() ; it++) {
        writer->writeString(it.key());
        it.value()->serialize(writer);
    }
}

void NBT::deserialize(IByteReader *reader) {
    int size = reader->readInt();
    for(int i = 0 ; i < size ; i ++) {
        QString key = reader->readString();
        Data* d = parseData(reader);
        data.insert(key, d);
    }
}

QString NBT::toString() {
    QString res = "{";
    auto it = data.begin();
    while (it != data.end()) {
        res = res % it.key() % ":" % it.value()->toString();
        if (++it == data.end()) {
            break;
        }
        res = res % ",";
    }
    return res % "}";
}

Data *NBT::copy() {
    NBT* res = new NBT();
    for (auto it = data.begin() ; it != data.end() ; it++) {
        res->data.insert(it.key(), it.value()->copy());
    }
    return res;
}

NBT::~NBT() {
    for (const auto &item: data) {
        delete item;
    }
}

IntData *NBT::putInt(const QString& key) {
    auto* d = data.value(key);
    if (!d) {
        d = new IntData();
        data.insert(key, d);
    } else if (d->type != INT){
        throw Error("NBT::putInt", "mismatched type");
    }
    return d->asInt();
}

ArrayData *NBT::putArray(const QString& key) {
    auto* d = data.value(key);
    if (!d) {
        d = new ArrayData();
        data.insert(key, d);
    } else if (d->type != ARR){
        throw Error("NBT::putArray", "mismatched type");
    }
    return d->asArray();
}

NBT *NBT::putCompound(const QString& key) {
    auto* d = data.value(key);
    if (!d) {
        d = new NBT();
        data.insert(key, d);
    } else if (d->type != ARR){
        throw Error("NBT::putArray", "mismatched type");
    }
    return d->asCompound();
}

Data *NBT::get(const QString &key) {
    return data.value(key);
}

bool NBT::contains(const QString &key) {
    return data.contains(key);
}
