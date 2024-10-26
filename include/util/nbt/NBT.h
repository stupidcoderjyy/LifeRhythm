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
#include "LongData.h"
#include "Plugin.h"

class CORE_API NBT final : public Data {
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
    LongData* putLong(const QString& key);
    NBT* putCompound(const QString& key);
    inline Data* get(const QString& key) const;
    inline QMap<QString, Data*>& get();
    inline bool contains(const QString& key) const;
    inline bool contains(const QString& key, int type) const;
    inline int getInt(const QString& key, int defaultVal = 0) const;
    inline QString getString(const QString& key, QString defaultVal = "") const;
    inline QVector<Data*>* getArr(const QString& key) const;
    inline float getFloat(const QString& key, float defaultVal = 0) const;
    inline bool getBool(const QString& key, bool defaultVal = false) const;
    inline long long getLong(const QString& key, long long defaultVal = 0) const;
    QString toString() override;
    ~NBT() override;
    static NBT* fromStringNbt(const QString &path);
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
protected:
    Data *copy() override;
};

inline Data *NBT::get(const QString &key) const {
    return data.value(key);
}
inline QMap<QString, Data*>& NBT::get() {
    return data;
}

inline bool NBT::contains(const QString &key) const {
    return data.contains(key);
}

inline bool NBT::contains(const QString &key, int type) const {
    auto* p = data.value(key);
    return p && p->type == type;
}

inline int NBT::getInt(const QString &key, int defaultVal) const {
    if (contains(key, INT)) {
        return get(key)->asInt()->get();
    }
    return defaultVal;
}

inline QString NBT::getString(const QString &key, QString defaultVal) const {
    if (contains(key, STRING)) {
        return get(key)->asString()->get();
    }
    return defaultVal;
}

inline QVector<Data *>* NBT::getArr(const QString &key) const {
    if (contains(key, ARR)) {
        return &get(key)->asArray()->get();
    }
    return nullptr;
}

inline float NBT::getFloat(const QString &key, float defaultVal) const {
    if (contains(key, FLOAT)) {
        return get(key)->asFloat()->get();
    }
    return defaultVal;
}

bool NBT::getBool(const QString &key, bool defaultVal) const {
    if (contains(key, BOOL)) {
        return get(key)->asBool()->get();
    }
    return defaultVal;
}

inline long long NBT::getLong(const QString &key, long long defaultVal) const {
    if (contains(key, LONG)) {
        return get(key)->asLong()->get();
    }
    return defaultVal;
}

#endif //LIFERHYTHM_NBT_H
