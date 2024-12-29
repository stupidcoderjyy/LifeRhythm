//
// Created by stupid_coder_jyy on 2024/2/17.
//

#include "AbstractInput.h"
#include "Error.h"

USING_LR

QString AbstractInput::readUtf() {
    switch (int b1 = read() & 0xFF; b1 >> 4) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7: {
            char data[2]{static_cast<char>(b1), '\0'};
            return {data};
        }
        case 12:
        case 13: {
            int b2 = read() & 0xFF;
            if ((b2 & 0xC0) != 0x80) {
                throw Error("AbstractInput::readUtf malformed format:" + QString::number((b1 << 8) | b2, 2));
            }
            char data[3]{static_cast<char>(b1), static_cast<char>(b2), '\0'};
            return {data};
        }
        case 14: {
            int b2 = read() & 0xFF;
            int b3 = read() & 0xFF;
            if ((b2 & 0xC0) != 0x80 || (b3 & 0xC0) != 0x80) {
                throw Error("AbstractInput::readUtf malformed format:" + QString::number((b1 << 16) | (b2 << 8) | b3, 2));
            }
            char data[4]{static_cast<char>(b1), static_cast<char>(b2), static_cast<char>(b3), '\0'};
            return {data};
        }
        default:
            throw Error("AbstractInput::readUtf malformed format:" + QString::number(b1, 2));
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
