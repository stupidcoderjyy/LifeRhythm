//
// Created by stupid_coder_jyy on 2024/2/17.
//

#include "StringInput.h"
#include "Error.h"

StringInput::StringInput(const QString& str) {
    next = 0;
    data = str.toUtf8();
}

int StringInput::read() {
    if (!available()) {
        throwInFunc("unavailable");
    }
    return data[next++];
}

int StringInput::forward() {
    return data[next];
}

void StringInput::mark() {
    marks << next;
}

void StringInput::removeMark() {
    marks.removeLast();
}

void StringInput::recover(bool consumeMark) {
    if (!marks.empty()) {
        next = consumeMark ? marks.takeLast() : marks.last();
    }
}

void StringInput::recover() {
    AbstractInput::recover();
}

QString StringInput::capture() {
    switch (marks.size()) {
        case 0:
            return "";
        case 1:
            return capture(next, marks.takeLast());
        default: {
            int end = marks.takeLast();
            return capture(end, marks.takeLast());
        }
    }
}

QString StringInput::capture(int end, int start) {
    return data.mid(start, end - start);
}

bool StringInput::available() const {
    return next < data.size();
}

int StringInput::retract(int count) {
    return AbstractInput::retract(count);
}

int StringInput::retract() {
    next--;
    if (next < 0) {
        throwInFunc("exceed retract limit");
    }
    return data[next];
}
