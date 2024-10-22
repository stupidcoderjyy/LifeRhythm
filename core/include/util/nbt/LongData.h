//
// Created by JYY on 24-10-15.
//

#ifndef LONGDATA_H
#define LONGDATA_H

#include "Data.h"

class CORE_API LongData final : public Data {
private:
    long long data = 0;
public:
    explicit LongData();
    LongData* setVal(long long val) {
        this->data = val;
        return this;
    }
    long long get() const {
        return data;
    }
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    QString toString() override;
    Data *copy() override;
};

#endif //LONGDATA_H
