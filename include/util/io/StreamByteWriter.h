//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_STREAMBYTEWRITER_H
#define LIFERHYTHM_STREAMBYTEWRITER_H

#include "IByteWriter.h"
#include <QDataStream>

class StreamByteWriter : public IByteWriter {
private:
    QDataStream* stream{};
public:
    explicit StreamByteWriter(QDataStream* stream);
    explicit StreamByteWriter(const QString& file);
    void write(char *src, int start, int length) override;
    void writeByte(int c) override;
    void writeInt(int i) override;
    void writeLong(long long int l) override;
    void writeDouble(double f) override;
    void writeBool(bool b) override;
    void writeShort(short s) override;
    void writeString(const QString &s) override;
    virtual ~StreamByteWriter();
};
#endif //LIFERHYTHM_STREAMBYTEWRITER_H
