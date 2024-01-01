//
// Created by stupid_coder_jyy on 2024/1/1.
//

#ifndef LIFERHYTHM_FLOATDATA_H
#define LIFERHYTHM_FLOATDATA_H

#include "Data.h"

class FloatData : public Data{
private:
    float data = 0;
public:
    explicit FloatData();
    FloatData* setVal(float val);
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    QString toString() override;
    Data *copy() override;
};

#endif //LIFERHYTHM_FLOATDATA_H
