//
// Created by stupid_coder_jyy on 2023/12/29.
//

#include "CompilerInput.h"
#include "StreamByteReader.h"
#include "StringByteReader.h"
#include "Error.h"
#include <utility>
#include <QFile>
#include <QIODevice>
#include <QDataStream>
#include "MemUtil.h"

QVector<int*> CompilerInput::removed{};
int CompilerInput::mPos = 0;
int** CompilerInput::reservedMarks = allocateArray(RESERVED_SIZE, MARK_LEN);

CompilerInput::CompilerInput(IByteReader *reader, QString filePath, int bufSize):
        filePath(std::move(filePath)), BufferedInput(reader, bufSize){
    rowBegin.append(0);
    markData.append(getData());
}

CompilerInput *CompilerInput::fromFile(const QString &file) {
    return fromFile(file, DEFAULT_BUFFER_SIZE);
}

CompilerInput *CompilerInput::fromFile(const QString &file, int bufSize) {
    return new CompilerInput(new StreamByteReader(file),file, bufSize);
}

CompilerInput *CompilerInput::fromString(const QString &str) {
    return new CompilerInput(new StringByteReader(str), "internal string");
}

int *CompilerInput::getData() {
    int* data = offerData();
    data[0] = row;
    data[1] = qMax(0, column);
    data[2] = columnSizes.empty() ? -1 : columnSizes.last();
    data[3] = rowBegin.last();
    data[4] = forward;
    return data;
}

int *CompilerInput::offerData() {
    if (removed.isEmpty()) {
        if (mPos == RESERVED_SIZE) {
            delete reservedMarks;
            reservedMarks = allocateArray(RESERVED_SIZE, MARK_LEN);
            mPos = 0;
        }
        return reservedMarks[mPos++];
    }
    return removed.takeLast();
}

CompilerInput::~CompilerInput() {
    while (!markData.empty()) {
        removeLastData();
    }
}

void CompilerInput::mark() {
    marks.append(forward);
    markData.append(getData());
}

void CompilerInput::removeMark() {
    marks.removeLast();
    removeLastData();
}

void CompilerInput::removeFirstData() {
    marks.removeLast();
    removed.append(markData.takeFirst());
}

int* CompilerInput::removeLastData() {
    int* m = markData.takeLast();
    removed.append(m);
    return m;
}

int CompilerInput::retract() {
    if (!markData.isEmpty() && markData.last()[4] == forward) {
        removeFirstData();
    }
    int b = BufferedInput::retract();
    switch (b) {
        case '\r':
            break;
        case '\n': {
            row--;
            column = columnSizes.takeLast();
            rowBegin.takeLast();
            break;
        }
        default: {
            column--;
            break;
        }
    }
    return b;
}

int CompilerInput::read() {
    int b = BufferedInput::read();
    switch (b) {
        case '\n': {
            row++;
            rowBegin.append(forward);
            columnSizes.append(column);
            column = 0;
            break;
        }
        case '\r':
            break;
        default: {
            column++;
            break;
        }
    }
    return b;
}

void CompilerInput::fillA() {
    BufferedInput::fillA();
    if (fillCount == 1) {
        return;
    }
    while (!rowBegin.empty() && rowBegin.first() < bufEndA) {
        rowBegin.removeFirst();
        if (columnSizes.empty()) {
            break;
        }
        columnSizes.removeFirst();
    }
    while (!markData.empty() && markData.first()[4] < bufEndA) {
        removeFirstData();
    }
}

void CompilerInput::fillB() {
    BufferedInput::fillB();
    while (!rowBegin.empty() && rowBegin.first() >= bufEndA) {
        rowBegin.removeFirst();
        columnSizes.removeFirst();
    }
    while (!markData.empty() && markData.first()[4] >= bufEndA) {
        removeFirstData();
    }
}

QString CompilerInput::currentLine() {
    int start = rowBegin.empty() ? 0 : rowBegin.last();
    mark();
    approach('\r');
    QString res = capture(forward, start);
    BufferedInput::recover();
    return res;
}

void CompilerInput::recover(bool consume) {
    BufferedInput::recover(consume);
    if (!markData.empty()) {
        recover(consume ? removeLastData() : markData.last());
    }
}

void CompilerInput::recoverFromData(int *data) {
    row = data[0];
    column = data[1];
    int cs = data[2];
    if (cs > 0) {
        while (columnSizes.last() != cs) {
            columnSizes.removeLast();
        }
    } else {
        columnSizes.clear();
    }
    int rb = data[3];
    while (rowBegin.last() != rb) {
        rowBegin.removeLast();
    }
}

CompileError CompilerInput::pointError(const QString &msg, int pos) {
    return CompileError(msg, row, currentLine(), filePath).setPos(pos);
}

CompileError CompilerInput::rangedError(const QString &msg, int end, int start) {
    return CompileError(msg, row, currentLine(), filePath).setPos(start, qMax(start, end));
}

CompileError CompilerInput::errorAtMark(const QString &msg) {
    if (marks.empty()) {
        return errorAtForward(msg);
    }
    int pos = markData.last()[1];
    removeMark();
    return pointError(msg, pos);
}

CompileError CompilerInput::errorAtForward(const QString &msg) {
    return pointError(msg, column);
}

CompileError CompilerInput::errorMarkToMark(const QString &msg) {
    switch (marks.size()) {
        case 0 :
            return errorAtForward(msg);
        case 1 :
            return errorMarkToForward(msg);
        default :
            int end = markData.last()[1];
            removeMark();
            int start = markData.last()[1];
            removeMark();
            return rangedError(msg, end, start);
    }
}

CompileError CompilerInput::errorMarkToForward(const QString &msg) {
    if (marks.empty()) {
        return errorAtForward(msg);
    }
    int start = markData.last()[1];
    removeMark();
    return rangedError(msg, column, start);
}

int CompilerInput::retract(int count) {
    return BufferedInput::retract(count);
}
