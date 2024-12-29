//
// Created by stupid_coder_jyy on 2023/5/30.
//

#ifndef PRACTICECONSOLE_IDATAINPUT_H
#define PRACTICECONSOLE_IDATAINPUT_H
#include "QString"
#include "Plugin.h"

class CORE_API IByteReader {
public:
    virtual int read(char * dest, int start, int length) = 0;
    virtual int readByte() = 0;
    virtual int readInt() = 0;
    virtual long long readLong() = 0;
    virtual double readDouble() = 0;
    virtual float readFloat() = 0;
    virtual bool readBool() = 0;
    virtual short readShort() = 0;
    virtual QString readString() = 0;
    virtual bool available() = 0;
    virtual ~IByteReader() = default;
};

#endif //PRACTICECONSOLE_IDATAINPUT_H
