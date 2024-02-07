//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "StreamByteReader.h"
#include <QDataStream>
#include <QFile>
#include "memory"
#include "Error.h"

StreamByteReader::StreamByteReader(const QString &fileName) {
    file = new QFile(fileName);
    if (!file->exists()) {
        throwInFunc("file not found:" + fileName);
    }
    file->open(QIODevice::ReadOnly);
    stream = new QDataStream(file);
}

int StreamByteReader::read(char *dest, int start, int length) {
    return stream->readRawData(dest + start, length);
}

int StreamByteReader::readByte() {
    uchar b;
    *stream >> b;
    return b;
}

int StreamByteReader::readInt() {
    int i;
    *stream >> i;
    return i;
}

long long int StreamByteReader::readLong() {
    long long l;
    *stream >> l;
    return l;
}

double StreamByteReader::readDouble() {
    double d;
    *stream >> d;
    return d;
}

float StreamByteReader::readFloat() {
    float d;
    *stream >> d;
    return d;
}

bool StreamByteReader::readBool() {
    uchar b;
    *stream >> b;
    return b != 0;
}

short StreamByteReader::readShort() {
    short s;
    *stream >> s;
    return s;
}

QString StreamByteReader::readString() {
    int len = readInt();
    char data[len];
    stream->readRawData(data, len);
    return const_cast<const char*>(data);
}

StreamByteReader::~StreamByteReader() {
    file->close();
    delete file;
    delete stream;
}
