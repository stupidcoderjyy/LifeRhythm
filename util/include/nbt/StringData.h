//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef STRINGDATA_H
#define STRINGDATA_H

#include "Data.h"
#include "Identifier.h"
#include "Plugin.h"

BEGIN_LR

class CORE_API StringData : public Data{
private:
    QString data{};
public:
    explicit StringData();

    inline StringData* setVal(const QString& val);
    inline const QString& get() const;
    inline Identifier getLoc() const;
protected:
    void serialize(IByteWriter *writer) override;
    void deserialize(IByteReader *reader) override;
    QString toString() override;
    Data *copy() override;
};

inline StringData * StringData::setVal(const QString &val) {
    this->data = val;
    return this;
}

inline const QString & StringData::get() const {
    return data;
}

inline Identifier StringData::getLoc() const {
    return Identifier(data);
}

END_LR

#endif //STRINGDATA_H
