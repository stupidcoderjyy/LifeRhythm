//
// Created by stupid_coder_jyy on 2024/2/17.
//

#include "AbstractInput.h"
#include "Error.h"

QString AbstractInput::readUtf() {
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

void AbstractInput::recover() {
    recover(true);
}

int AbstractInput::retract(int count) {
    if (count <= 0) {
        return -1;
    }
    count--;
    for (int i = 0 ; i < count ; i ++) {
        retract();
    }
    return retract();
}

int AbstractInput::approach(int ch) {
    while (available()) {
        if (forward() == ch) {
            return ch;
        }
        read();
    }
    return -1;
}

int AbstractInput::approach(int ch1, int ch2) {
    while (available()) {
        if (forward() == ch1) {
            return ch1;
        }
        if (forward() == ch2) {
            return ch2;
        }
        read();
    }
    return -1;
}

int AbstractInput::approach(int ch1, int ch2, int ch3) {
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
        read();
    }
    return -1;
}

void prepareBitClazz(bool* clazz, std::initializer_list<int>& list) {
    memset(clazz, false, 128);
    for (const auto &ch : list) {
        if (ch >= 0 && ch < 128) {
            clazz[ch] = true;
        }
    }
}

int AbstractInput::approach(std::initializer_list<int> list) {
    prepareBitClazz(bitClazz, list);
    while (available()) {
        int ch = forward();
        if (ch >= 0 && bitClazz[ch]) {
            return ch;
        }
        read();
    }
    return -1;
}

int AbstractInput::find(int ch) {
    while (available()) {
        if (read() == ch) {
            return ch;
        }
    }
    return -1;
}

int AbstractInput::find(int ch1, int ch2) {
    while (available()) {
        int cur = read();
        if (cur == ch1 || cur == ch2) {
            return cur;
        }
    }
    return -1;
}

int AbstractInput::find(int ch1, int ch2, int ch3) {
    while (available()) {
        int cur = read();
        if (cur == ch1 || cur == ch2 || cur == ch3) {
            return cur;
        }
    }
    return -1;
}

int AbstractInput::find(std::initializer_list<int> list) {
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

int AbstractInput::skip(int ch) {
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

int AbstractInput::skip(int ch1, int ch2) {
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

int AbstractInput::skip(int ch1, int ch2, int ch3) {
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

int AbstractInput::skip(std::initializer_list<int> list) {
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
