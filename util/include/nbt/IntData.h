//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef INTDATA_H
#define INTDATA_H

#include "Data.h"

BEGIN_LR

class CORE_API IntData final : public Data{
private:
    int data = 0;
public:
    explicit IntData();
    IntData* setVal(int val) {
        this->data = val;
        return this;
    }
    int get() const {
        return data;
    }
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    QString toString() override;
    Data *copy() override;
};

END_LR

#endif //INTDATA_H
