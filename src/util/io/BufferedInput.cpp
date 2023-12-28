//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "BufferedInput.h"
#include "Preconditions.h"
#include "Error.h"
#include "StreamByteReader.h"
#include <QFile>
#include <QDataStream>

BufferedInput::BufferedInput(IByteReader *reader, int bufSize):reader(reader) {
    Preconditions::checkIndexRange(1, MAX_BUFFER_SIZE, bufSize,"BufferedInput::BufferedInput",
                                   QString::asprintf("invalid buffer size, required:(0, %d], provided:%d", MAX_BUFFER_SIZE, bufSize));
    Preconditions::checkNotNull(reader, "BufferedInput::BufferedInput","null reader");
    buffer = new char[bufSize * 2];
    bitClazz = new char[128];
    bufEndA = bufSize;
    bufEndB = bufSize * 2;
    fillA();
    mark();
}

BufferedInput::BufferedInput(IByteReader *reader): BufferedInput(reader, DEFAULT_BUFFER_SIZE) {
}

BufferedInput *BufferedInput::fromFile(const QString &file) {
    return fromFile(file, DEFAULT_BUFFER_SIZE);
}

BufferedInput *BufferedInput::fromFile(const QString &file, int bufSize) {
    auto* f = new QFile(file);
    if (!f->exists()) {
        throw Error("BufferedInput::fromFile", "File not found:" + file);
    }
    f->open(QIODevice::ReadWrite);
    return new BufferedInput(new StreamByteReader(new QDataStream(f)), bufSize);
}

bool BufferedInput::available() const {
    return inputEnd < 0 || forward != inputEnd;
}

int BufferedInput::read() {
    Preconditions::checkState(available(), "BufferedInput::read", "not available");
    char result = buffer[forward++];
    if (forward == bufEndB) {
        forward = 0;
        if ((fillCount & 1) == 0) {
            fillA();
        }
    } else if (forward == bufEndA) {
        if ((fillCount & 1) == 1) {
            fillB();
        }
    }
    return result;
}

void BufferedInput::fillA() {
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
    marks.append(forward); // addFirst
}

void BufferedInput::removeMark() {
    marks.removeLast();
}

int BufferedInput::retract() {
    Preconditions::checkState(buffer, "BufferedInput::retract", "closed");
    if (forward == 0) {
        if (fillCount == 1 || (fillCount & 1) == 0) {
            throw Error("BufferedInput::retract", "exceed retract limit");
        }
        forward = bufEndB - 1;
    } else if (forward == bufEndA) {
        if ((fillCount & 1) == 1) {
            throw Error("BufferedInput::retract", "exceed retract limit");
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
    Preconditions::checkState(buffer, "BufferedInput::capture", "closed");
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
    char* temp;
    int len;
    if (start < end) {
        len = end - start;
        temp = new char[len + 1];
        memcpy(temp, buffer + start, len);
    } else {
        int lenB = bufEndB - start;
        len = lenB + end;
        temp = new char[len + 1];
        memcpy(temp, buffer + start, lenB);
        memcpy(temp + lenB, buffer, end);
    }
    temp[len] = '\0';
    QString res = temp;
    delete[] temp;
    return res;
}

int BufferedInput::approach(int ch) {
    while (available()) {
        if (buffer[forward] == ch) {
            return ch;
        }
    }
    return -1;
}

int BufferedInput::approach(std::initializer_list<int> list) {
    memset(bitClazz, false, 128);
    for (const auto &ch : list) {
        if (ch >= 0 && ch < 128) {
            bitClazz[ch] = true;
        }
    }
    while (available()) {
        if (bitClazz[buffer[forward]]) {
            return buffer[forward];
        }
    }
    return -1;
}

BufferedInput::~BufferedInput() {
    delete reader;
    delete[] buffer;
    delete[] bitClazz;
}
