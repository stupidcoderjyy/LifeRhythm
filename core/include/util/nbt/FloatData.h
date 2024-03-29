//
// Created by stupid_coder_jyy on 2024/1/1.
//

#ifndef LIFERHYTHM_FLOATDATA_H
#define LIFERHYTHM_FLOATDATA_H

#include "Data.h"

class FloatData : public Data{
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

#endif //LIFERHYTHM_FLOATDATA_H
