//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_INTDATA_H
#define LIFERHYTHM_INTDATA_H

#include "Data.h"

class IntData : public Data{
private:
    int data = 0;
public:
    explicit IntData();
    inline IntData* setVal(int val) {
        this->data = val;
        return this;
    }
    inline int get() const {
        return data;
    }
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    QString toString() override;
    Data *copy() override;
};
#endif //LIFERHYTHM_INTDATA_H
