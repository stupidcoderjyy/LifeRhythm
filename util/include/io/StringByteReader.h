//
// Created by stupid_coder_jyy on 2024/1/15.
//

#ifndef STRINGBYTEREADER_H
#define STRINGBYTEREADER_H

#include "IByteReader.h"
#include "Plugin.h"

BEGIN_LR

class CORE_API StringByteReader final : public IByteReader {
private:
    QByteArray data;
    int next{};
public:
    explicit StringByteReader(const QString& val);
    int read(char *dest, int off, int length) override;
    int readByte() override;
    int readInt() override;
    long long int readLong() override;
    double readDouble() override;
    float readFloat() override;
    bool readBool() override;
    short readShort() override;
    QString readString() override;
    bool available() override;
};

END_LR

#endif //STRINGBYTEREADER_H
