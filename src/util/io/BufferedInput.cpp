//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "BufferedInput.h"
#include "Error.h"
#include "StreamByteReader.h"
#include "StringByteReader.h"
#include <QFile>
#include <QDataStream>

BufferedInput::BufferedInput(IByteReader *reader, int bufSize):reader(reader) {
    if (bufSize < 1 || bufSize > MAX_BUFFER_SIZE) {
        throwInFunc(QString::asprintf(
                "invalid buffer size, required:(0, %d], provided:%d", MAX_BUFFER_SIZE, bufSize));
    }
    if (!reader) {
        throwInFunc("null reader");
    }
    buffer = new char[bufSize * 2];
    bitClazz = new char[128];
    bufEndA = bufSize;
    bufEndB = bufSize * 2;
    _fillA();
    _mark0();
}

BufferedInput::BufferedInput(IByteReader *reader): BufferedInput(reader, DEFAULT_BUFFER_SIZE) {
}

BufferedInput *BufferedInput::fromFile(const QString &file) {
    return fromFile(file, DEFAULT_BUFFER_SIZE);
}

BufferedInput *BufferedInput::fromFile(const QString &file, int bufSize) {
    return new BufferedInput(new StreamByteReader(file), bufSize);
}

BufferedInput *BufferedInput::fromString(const QString &str) {
    return new BufferedInput(new StringByteReader(str));
}

bool BufferedInput::available() const {
    return inputEnd < 0 || forward != inputEnd;
}

int BufferedInput::read() {
    if (!available()) {
        throwInFunc("not available");
    }
    char result = buffer[forward++];
    if (forward == bufEndB) {
        forward = 0;
        if ((fillCount & 1) == 0) {
            _fillA();
        }
    } else if (forward == bufEndA) {
        if ((fillCount & 1) == 1) {
            fillB();
        }
    }
    return result;
}

QString BufferedInput::readUtf() {
    int b1 = read() & 0xFF;
    switch (b1 >> 4) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7: {
            char data[2]{(char)b1, '\0'};
            return {data};
        }
        case 12:
        case 13: {
            int b2 = read() & 0xFF;
            if ((b2 & 0xC0) != 0x80) {
                throwInFunc("malformed format:" + QString::number((b1 << 8) | b2, 2));
            }
            char data[3]{(char)b1, (char)b2, '\0'};
            return {data};
        }
        case 14: {
            int b2 = read() & 0xFF;
            int b3 = read() & 0xFF;
            if ((b2 & 0xC0) != 0x80 || (b3 & 0xC0) != 0x80) {
                throwInFunc("malformed format:" + QString::number((b1 << 16) | (b2 << 8) | b3, 2));
            }
            char data[4]{(char) b1, (char) b2, (char) b3, '\0'};
            return {data};
        }
        default:
            throwInFunc("malformed format:" + QString::number(b1, 2));
    }
}

void BufferedInput::fillA() {
    _fillA();
}

void BufferedInput::_fillA() {
    fillCount++;
    int size = reader->read(buffer,0,bufEndA);
    if (size < bufEndA) {
        inputEnd = size;
    }
    while (!marks.empty() && marks.first() < bufEndA) {
        marks.removeFirst();
    }
}

void BufferedInput::fillB() {
    fillCount++;
    int size = reader->read(buffer, bufEndA, bufEndA);
    if (size < bufEndA) {
        inputEnd = bufEndA + size;
    }
    while (!marks.empty() && marks.first() >= bufEndA) {
        marks.removeFirst();
    }
}

void BufferedInput::mark() {
    _mark0();
}

void BufferedInput::_mark0() {
    marks.append(forward); // addFirst
}

void BufferedInput::removeMark() {
    marks.removeLast();
}

void BufferedInput::recover(bool consume) {
    if (!marks.isEmpty()) {
        forward = consume ? marks.takeLast() : marks.last();
    }
}

void BufferedInput::recover() {
    recover(true);
}

int BufferedInput::retract() {
    if (!buffer) {
        throwInFunc("closed");
    }
    if (forward == 0) {
        if (fillCount == 1 || (fillCount & 1) == 0) {
            throwInFunc("exceed retract limit");
        }
        forward = bufEndB - 1;
    } else if (forward == bufEndA) {
        if ((fillCount & 1) == 1) {
            throwInFunc("exceed retract limit");
        }
        forward--;
    } else {
        forward--;
    }
    return buffer[forward];
}

int BufferedInput::retract(int count) {
    if (count <= 0) {
        return -1;
    }
    count--;
    for (int i = 0 ; i < count ; i ++) {
        retract();
    }
    return retract();
}

QString BufferedInput::capture() {
    if (!buffer) {
        throwInFunc("closed");
    }
    switch (marks.size()) {
        case 1: {
            int start = marks.last();
            removeMark();
            return capture(forward, start);
        }
        case 0:
            return "";
        default: {
            int end = marks.last();
            removeMark();
            int start = marks.last();
            removeMark();
            return capture(end, start);
        }
    }
}

QString BufferedInput::capture(int end, int start) {
    int len;
    if (start < end) {
        len = end - start;
        char temp[len + 1];
        memcpy(temp, buffer + start, len);
        temp[len] = '\0';
        return temp;
    } else {
        int lenB = bufEndB - start;
        len = lenB + end;
        char temp[len + 1];
        memcpy(temp, buffer + start, lenB);
        memcpy(temp + lenB, buffer, end);
        temp[len] = '\0';
        return temp;
    }
}

int BufferedInput::approach(int ch) {
    while (available()) {
        if (buffer[forward] == ch) {
            return ch;
        }
        read();
    }
    return -1;
}

int BufferedInput::approach(int ch1, int ch2) {
    while (available()) {
        if (buffer[forward] == ch1) {
            return ch1;
        }
        if (buffer[forward] == ch2) {
            return ch2;
        }
        read();
    }
    return -1;
}

int BufferedInput::approach(int ch1, int ch2, int ch3) {
    while (available()) {
        if (buffer[forward] == ch1) {
            return ch1;
        }
        if (buffer[forward] == ch2) {
            return ch2;
        }
        if (buffer[forward] == ch3) {
            return ch3;
        }
        read();
    }
    return -1;
}

void prepareBitClazz(char* clazz, std::initializer_list<int>& list) {
    memset(clazz, false, 128);
    for (const auto &ch : list) {
        if (ch >= 0 && ch < 128) {
            clazz[ch] = true;
        }
    }
}

int BufferedInput::approach(std::initializer_list<int> list) {
    prepareBitClazz(bitClazz, list);
    while (available()) {
        char ch = buffer[forward];
        if (ch >= 0 && bitClazz[ch]) {
            return buffer[forward];
        }
        read();
    }
    return -1;
}

int BufferedInput::find(int ch) {
    while (available()) {
        if (read() == ch) {
            return ch;
        }
    }
    return -1;
}

int BufferedInput::find(int ch1, int ch2) {
    while (available()) {
        int cur = read();
        if (cur == ch1 || cur == ch2) {
            return cur;
        }
    }
    return -1;
}

int BufferedInput::find(int ch1, int ch2, int ch3) {
    while (available()) {
        int cur = read();
        if (cur == ch1 || cur == ch2 || cur == ch3) {
            return cur;
        }
    }
    return -1;
}

int BufferedInput::find(std::initializer_list<int> list) {
    prepareBitClazz(bitClazz, list);
    while (available()) {
        int ch = read();
        if (ch >= 0 && bitClazz[ch]) {
            return ch;
        }
        read();
    }
    return -1;
}

int BufferedInput::skip(int ch) {
    int pre = -1;
    while (available()) {
        int b = read();
        if (b != ch) {
            retract();
            break;
        }
        pre = b;
    }
    return pre;
}

int BufferedInput::skip(int ch1, int ch2) {
    int pre = -1;
    while (available()) {
        int b = read();
        if (b != ch1 && b != ch2) {
            retract();
            break;
        }
        pre = b;
    }
    return pre;
}

int BufferedInput::skip(int ch1, int ch2, int ch3) {
    int pre = -1;
    while (available()) {
        int b = read();
        if (b != ch1 && b != ch2 && b != ch3) {
            retract();
            break;
        }
        pre = b;
    }
    return pre;
}

int BufferedInput::skip(std::initializer_list<int> list) {
    prepareBitClazz(bitClazz, list);
    int pre = -1;
    while (available()) {
        int b = read();
        if (b < 0 || !bitClazz[b]) {
            retract();
            break;
        }
        pre = b;
    }
    return pre;
}

BufferedInput::~BufferedInput() {
    delete reader;
    delete[] buffer;
    delete[] bitClazz;
}
