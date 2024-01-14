//
// Created by stupid_coder_jyy on 2024/1/1.
//

#ifndef LIFERHYTHM_BOOLDATA_H
#define LIFERHYTHM_BOOLDATA_H

#include "Data.h"

class BoolData : public Data{
private:
    bool data = false;
public:
    explicit BoolData();
    BoolData* setVal(bool val);
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    QString toString() override;
    Data *copy() override;
};

#endif
