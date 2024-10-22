//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_STRINGDATA_H
#define LIFERHYTHM_STRINGDATA_H

#include "Data.h"
#include "Identifier.h"
#include "Plugin.h"

class CORE_API StringData : public Data{
private:
    QString data{};
public:
    explicit StringData();
    inline StringData* setVal(const QString& val) {
        this->data = val;
        return this;
    }
    inline const QString& get() const {
        return data;
    }
    inline Identifier getLoc() const {
        return Identifier(data);
    }
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    QString toString() override;
    Data *copy() override;
};
#endif //LIFERHYTHM_STRINGDATA_H
