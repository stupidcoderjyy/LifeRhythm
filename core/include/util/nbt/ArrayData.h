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
    ArrayData* add(int val);
    ArrayData* add(ArrayData* val);
    ArrayData* add(const QString& val);
    ArrayData* add(float val);
    ArrayData* add(NBT* val);
    QString toString() override;
    Data *copy() override;
    ~ArrayData() override;
    QVector<Data*>& get() {
        return data;
    }
    void fillString(QStringList& target);
    void fillString(QStringList& target, int len, const QString& defaultVal = "");
    void fillInt(int* arr, int len);
    void fillInt(QVector<int>& target, int len = -1, int defaultVal = 0);
    void fillFloat(float* arr, int len);
    void fillBool(bool* arr, int len);
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
};
#endif //LIFERHYTHM_ARRAYDATA_H
