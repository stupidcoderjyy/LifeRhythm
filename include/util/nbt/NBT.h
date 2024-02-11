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
    inline Data* get(const QString& key) {
        return data.value(key);
    }
    inline QMap<QString, Data*>& get() {
        return data;
    }
    bool contains(const QString& key);
    bool contains(const QString& key, int type);
    int getInt(const QString& key, int defaultVal = 0);
    QString getString(const QString& key, QString defaultVal = "");
    QVector<Data*>* getArr(const QString& key);
    float getFloat(const QString& key, float defaultVal = 0);
    bool getBool(const QString& key, bool defaultVal = false);
    QString toString() override;
    ~NBT() override;
    static NBT* fromStringNbt(const QString &path);
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    Data *copy() override;
};
#endif //LIFERHYTHM_NBT_H
