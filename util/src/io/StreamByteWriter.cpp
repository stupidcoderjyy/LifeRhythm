//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "StreamByteWriter.h"
#include <QFile>
#include "Error.h"

USING_LR

StreamByteWriter::StreamByteWriter(QDataStream *stream) {
    if (!stream) {
        throw Error("StreamByteWriter::StreamByteWriter null stream");
    }
    this->stream = stream;
}

StreamByteWriter::StreamByteWriter(const QString &file) {
    auto* f = new QFile(file);
    f->open(QIODevice::WriteOnly);
    stream = new QDataStream(f);
}

void StreamByteWriter::write(char *src, int start, int length) {
    stream->writeRawData(src + start, length);
}

void StreamByteWriter::writeByte(int c) {
    *stream << static_cast<qint8>(c);
}

void StreamByteWriter::writeInt(int i) {
    *stream << i;
}

void StreamByteWriter::writeLong(long long int l) {
    *stream << l;
}

void StreamByteWriter::writeDouble(double f) {
    *stream << f;
}

void StreamByteWriter::writeFloat(float f) {
    *stream << f;
}

void StreamByteWriter::writeBool(bool b) {
    *stream << b;
}

void StreamByteWriter::writeShort(short s) {
    *stream << s;
}

void StreamByteWriter::writeString(const QString &s) {
    QByteArray d = s.toUtf8().data();
    int l = d.length();
    writeInt(l);
    write(d.data(), 0, l);
}

StreamByteWriter::~StreamByteWriter() {
    stream->device()->close();
    delete stream->device();
    delete stream;
}
