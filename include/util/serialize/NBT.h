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

class NBT : public Data {
    friend class NBTUtil;
    friend class PropertyItemList;
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
    Data* get(const QString& key);
    bool contains(const QString& key);
    QString toString() override;
    ~NBT() override;
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    Data *copy() override;
};
#endif //LIFERHYTHM_NBT_H
