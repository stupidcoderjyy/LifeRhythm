//
// Created by stupid_coder_jyy on 2023/12/28.
//
#include "NBT.h"
#include <QStringBuilder>
#include "Error.h"
#include "nbt/parser/Parser.h"

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

StringData *NBT::putString(const QString &key) {
    auto* d = data.value(key);
    if (!d) {
        d = new StringData();
        data.insert(key, d);
    } else if (d->type != STRING){
        throw Error("NBT::putString", "mismatched type");
    }
    return d->asString();
}

FloatData *NBT::putFloat(const QString &key) {
    auto* d = data.value(key);
    if (!d) {
        d = new FloatData();
        data.insert(key, d);
    } else if (d->type != FLOAT){
        throw Error("NBT::putFloat", "mismatched type");
    }
    return d->asFloat();
}

BoolData *NBT::putBool(const QString &key) {
    auto* d = data.value(key);
    if (!d) {
        d = new BoolData();
        data.insert(key, d);
    } else if (d->type != BOOL){
        throw Error("NBT::putBool", "mismatched type");
    }
    return d->asBool();
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

bool NBT::contains(const QString &key) {
    return data.contains(key);
}

bool NBT::contains(const QString &key, int type) {
    auto* p = data.value(key);
    return p && p->type == type;
}

NBT *NBT::fromStringNbt(const QString &path) {
    return snbt::Parser::parse(path);
}

int NBT::getInt(const QString &key, int defaultVal) {
    if (contains(key, INT)) {
        return get(key)->asInt()->get();
    }
    return defaultVal;
}

QString NBT::getString(const QString &key, QString defaultVal) {
    if (contains(key, STRING)) {
        return get(key)->asString()->get();
    }
    return defaultVal;
}

QVector<Data *>* NBT::getArr(const QString &key) {
    if (contains(key, ARR)) {
        return &get(key)->asArray()->get();
    }
    return nullptr;
}

float NBT::getFloat(const QString &key, float defaultVal) {
    if (contains(key, FLOAT)) {
        return get(key)->asFloat()->get();
    }
    return defaultVal;
}

bool NBT::getBool(const QString &key, bool defaultVal) {
    if (contains(key, BOOL)) {
        return get(key)->asBool()->get();
    }
    return defaultVal;
}
