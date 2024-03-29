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
    inline BoolData* setVal(bool val) {
        this->data = val;
        return this;
    }
    inline bool get() const {
        return data;
    }
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    QString toString() override;
    Data *copy() override;
};

#endif
