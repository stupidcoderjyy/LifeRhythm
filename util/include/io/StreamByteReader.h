//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef STREAMBYTEREADER_H
#define STREAMBYTEREADER_H

#include "IByteReader.h"
#include "memory"

class QFile;

BEGIN_LR

class CORE_API StreamByteReader final : public IByteReader {
private:
    QDataStream* stream = nullptr;
    QFile* file;
public:
    explicit StreamByteReader(const QString& file);
    int read(char *dest, int start, int length) override;
    int readByte() override;
    int readInt() override;
    long long int readLong() override;
    double readDouble() override;
    float readFloat() override;
    bool readBool() override;
    short readShort() override;
    QString readString() override;
    bool available() override;
    ~StreamByteReader() override;
};

END_LR

#endif //STREAMBYTEREADER_H
