//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_NBT_H
#define LIFERHYTHM_NBT_H

#include "Data.h"
#include "IntData.h"
#include "ArrayData.h"
#include "StringData.h"
#include "FloatData.h"
#include "BoolData.h"
#include <QMap>
#include "Identifier.h"

class NBT : public Data {
    friend class NBTUtil;
private:
    QMap<QString, Data*> data{};
public:
    explicit NBT();
    IntData* putInt(const QString& key);
    ArrayData* putArray(const QString& key);
    StringData* putString(const QString& key);
    FloatData* putFloat(const QString& key);
    BoolData* putBool(const QString& key);
    NBT* putCompound(const QString& key);
    inline Data* get(const QString& key);
    inline QMap<QString, Data*>& get();
    inline bool contains(const QString& key);
    inline bool contains(const QString& key, int type);
    inline int getInt(const QString& key, int defaultVal = 0);
    inline QString getString(const QString& key, QString defaultVal = "");
    inline QVector<Data*>* getArr(const QString& key);
    inline float getFloat(const QString& key, float defaultVal = 0);
    inline bool getBool(const QString& key, bool defaultVal = false);
    QString toString() override;
    ~NBT() override;
    static NBT* fromStringNbt(const QString &path);
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
protected:
    Data *copy() override;
};

inline Data *NBT::get(const QString &key) {
    return data.value(key);
}
inline QMap<QString, Data*>& NBT::get() {
    return data;
}

inline bool NBT::contains(const QString &key) {
    return data.contains(key);
}

inline bool NBT::contains(const QString &key, int type) {
    auto* p = data.value(key);
    return p && p->type == type;
}

inline int NBT::getInt(const QString &key, int defaultVal) {
    if (contains(key, INT)) {
        return get(key)->asInt()->get();
    }
    return defaultVal;
}

inline QString NBT::getString(const QString &key, QString defaultVal) {
    if (contains(key, STRING)) {
        return get(key)->asString()->get();
    }
    return defaultVal;
}

inline QVector<Data *>* NBT::getArr(const QString &key) {
    if (contains(key, ARR)) {
        return &get(key)->asArray()->get();
    }
    return nullptr;
}

inline float NBT::getFloat(const QString &key, float defaultVal) {
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

#endif //LIFERHYTHM_NBT_H
