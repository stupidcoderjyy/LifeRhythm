//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "BufferedInput.h"
#include "Error.h"
#include "StreamByteReader.h"
#include "StringByteReader.h"
#include <QFile>
#include <QDataStream>

USING_LR

BufferedInput::BufferedInput(IByteReader *reader, int bufSize):AbstractInput(), reader(reader) {
    if (bufSize < 1 || bufSize > MAX_BUFFER_SIZE) {
        throw Error(QString::asprintf(
                "BufferedInput::BufferedInput invalid buffer size, required:(0, %d], provided:%d", MAX_BUFFER_SIZE, bufSize));
    }
    if (!reader) {
        throw Error("BufferedInput::BufferedInput null reader");
    }
    buffer = new char[bufSize * 2];
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
    return inputEnd < 0 || next != inputEnd;
}

int BufferedInput::read() {
    if (!available()) {
        throw Error("BufferedInput::read not available");
    }
    char result = buffer[next++];
    if (next == bufEndB) {
        next = 0;
        if ((fillCount & 1) == 0) {
            _fillA();
        }
    } else if (next == bufEndA) {
        if ((fillCount & 1) == 1) {
            fillB();
        }
    }
    return result;
}

int BufferedInput::forward() {
    return buffer[next];
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
    marks.append(next); // addFirst
}

void BufferedInput::removeMark() {
    marks.removeLast();
}

void BufferedInput::recover(bool consume) {
    if (!marks.isEmpty()) {
        next = consume ? marks.takeLast() : marks.last();
    }
}

void BufferedInput::recover() {
    AbstractInput::recover();
}

int BufferedInput::retract() {
    if (!buffer) {
        throw Error("BufferedInput::retract closed");
    }
    if (next == 0) {
        if (fillCount == 1 || (fillCount & 1) == 0) {
            throw Error("BufferedInput::retract exceed retract limit");
        }
        next = bufEndB - 1;
    } else if (next == bufEndA) {
        if ((fillCount & 1) == 1) {
            throw Error("BufferedInput::retract exceed retract limit");
        }
        next--;
    } else {
        next--;
    }
    return buffer[next];
}

int BufferedInput::retract(int count) {
    return AbstractInput::retract(count);
}

QString BufferedInput::capture() {
    if (!buffer) {
        throw Error("BufferedInput::capture closed");
    }
    switch (marks.size()) {
        case 1: {
            int start = marks.last();
            removeMark();
            return capture(next, start);
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

QString BufferedInput::capture(int end, int start) const {
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

BufferedInput::~BufferedInput() {
    delete reader;
    delete[] buffer;
}
