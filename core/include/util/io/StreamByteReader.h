//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_STREAMBYTEREADER_H
#define LIFERHYTHM_STREAMBYTEREADER_H

#include "IByteReader.h"
#include "memory"

class QFile;

class StreamByteReader final : public IByteReader {
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
    ~StreamByteReader() override;
};

#endif //LIFERHYTHM_STREAMBYTEREADER_H
