//
// Created by stupid_coder_jyy on 2023/5/30.
//

#ifndef PRACTICECONSOLE_IDATAOUTPUT_H
#define PRACTICECONSOLE_IDATAOUTPUT_H

#include "QString"

class IByteWriter {
public:
    virtual void write(char* src, int start, int length) = 0;
    virtual void writeByte(int c) = 0;
    virtual void writeInt(int i) = 0;
    virtual void writeLong(long long l) = 0;
    virtual void writeDouble(double f) = 0;
    virtual void writeBool(bool b) = 0;
    virtual void writeShort(short s) = 0;
    virtual void writeString(const QString& s) = 0;
};


#endif //PRACTICECONSOLE_IDATAOUTPUT_H
