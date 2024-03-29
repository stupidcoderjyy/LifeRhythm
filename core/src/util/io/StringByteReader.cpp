//
// Created by stupid_coder_jyy on 2024/1/15.
//
#include "StringByteReader.h"
#include "Error.h"
#include <utility>

StringByteReader::StringByteReader(const QString& str):data(str.toUtf8()) {
}

int StringByteReader::read(char *dest, int off, int length) {
    int actualLen = qMin(length, data.length() - next);
    memcpy(dest + off, data.data() + next, actualLen);
    next += actualLen;
    return actualLen;
}

int StringByteReader::readByte() {
    return 0;
}

int StringByteReader::readInt() {
    return 0;
}

long long int StringByteReader::readLong() {
    return 0;
}

double StringByteReader::readDouble() {
    return 0;
}

float StringByteReader::readFloat() {
    return 0;
}

bool StringByteReader::readBool() {
    return false;
}

short StringByteReader::readShort() {
    return 0;
}

QString StringByteReader::readString() {
    return "";
}
