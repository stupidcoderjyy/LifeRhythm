//
// Created by stupid_coder_jyy on 2024/1/1.
//

#ifndef FLOATDATA_H
#define FLOATDATA_H

#include "Data.h"

BEGIN_LR

class CORE_API FloatData : public Data{
private:
    float data{};
public:
    explicit FloatData();
    inline FloatData* setVal(float val) {
        data = val;
        return this;
    }
    inline float get() const {
        return data;
    }
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    QString toString() override;
    Data *copy() override;
};

END_LR

#endif //FLOATDATA_H
