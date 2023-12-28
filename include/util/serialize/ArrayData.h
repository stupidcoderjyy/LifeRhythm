//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_ARRAYDATA_H
#define LIFERHYTHM_ARRAYDATA_H

#include "Data.h"
#include "QVector"
class NBT;

class ArrayData : public Data {
private:
    QVector<Data*> data{};
public:
    explicit ArrayData();
    ArrayData* addInt(int val);
    ArrayData* addArray(ArrayData* val);
    ArrayData* addTag(NBT* val);
    QString toString() override;
    Data *copy() override;
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    ~ArrayData() override;
};
#endif //LIFERHYTHM_ARRAYDATA_H
