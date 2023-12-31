//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_STRINGDATA_H
#define LIFERHYTHM_STRINGDATA_H

#include "Data.h"

class StringData : public Data{
private:
    QString data{};
public:
    explicit StringData();
    StringData* setVal(const QString& val);
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    QString toString() override;
    Data *copy() override;
};
#endif //LIFERHYTHM_INTDATA_H
