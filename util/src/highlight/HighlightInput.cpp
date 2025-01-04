//
// Created by JYY on 25-1-3.
//

#include "HighlightInput.h"

USING_LR

HighlightInput::HighlightInput(const QString &blockText): StringInput(blockText), p() {
}

int HighlightInput::read() {
    p++;
    return StringInput::read();
}

QString HighlightInput::readUtf() {
    int prev = p;
    auto res = StringInput::readUtf();
    p = prev + 1;
    return res;
}

void HighlightInput::retractUtf(int count) {
    int ch;
    int bytes = 0;
    for (int i = 0; i < count; i++) {
        do {
            ch = retract() & 0xFF;
            bytes++;
        } while (ch >> 6 == 2);
    }
    p += bytes - count;
}

int HighlightInput::retract() {
    p--;
    return StringInput::retract();
}

int HighlightInput::retract(int count) {
    return StringInput::retract(count);
}

int HighlightInput::approachUtf(int ch) {
    while (available()) {
        if (forward() == ch) {
            return ch;
        }
        readUtf();
    }
    return -1;
}

int HighlightInput::approachUtf(int ch1, int ch2) {
    while (available()) {
        if (forward() == ch1) {
            return ch1;
        }
        if (forward() == ch2) {
            return ch2;
        }
        readUtf();
    }
    return -1;
}

int HighlightInput::approachUtf(int ch1, int ch2, int ch3) {
    while (available()) {
        int ch = forward();
        if (ch == ch1) {
            return ch1;
        }
        if (ch == ch2) {
            return ch2;
        }
        if (ch == ch3) {
            return ch3;
        }
        readUtf();
    }
    return -1;
}

int HighlightInput::approachUtf(const std::initializer_list<int> &list) {
    prepareBitClazz(bitClazz, list);
    while (available()) {
        int ch = forward();
        if (ch < 0) {
            readUtf();
        } else if (bitClazz[ch]) {
            return ch;
        }
        read();
    }
    return -1;
}
