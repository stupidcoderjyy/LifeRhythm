//
// Created by stupid_coder_jyy on 2024/2/17.
//

#ifndef ABSTRACTINPUT_H
#define ABSTRACTINPUT_H

#include <QString>

#include "Namespaces.h"
#include "Plugin.h"

BEGIN_LR

class CORE_API AbstractInput {
private:
    bool bitClazz[128]{};
public:
    virtual ~AbstractInput() = default;
    virtual int read() = 0;
    virtual int forward() = 0;
    virtual QString readUtf();
    virtual void mark() = 0;
    virtual void removeMark() = 0;
    virtual void recover(bool consumeMark) = 0;
    virtual void recover();
    virtual QString capture() = 0;
    virtual bool available() const = 0;
    virtual int retract(int count);
    virtual int retract() = 0;
    inline int approach(int ch);
    inline int approach(int ch1, int ch2);
    inline int approach(int ch1, int ch2, int ch3);
    inline int approach(std::initializer_list<int> list);
    inline int find(int ch);
    inline int find(int ch1, int ch2);
    inline int find(int ch1, int ch2, int ch3);
    inline int find(std::initializer_list<int> list);
    inline int skip(int ch);
    inline int skip(int ch1, int ch2);
    inline int skip(int ch1, int ch2, int ch3);
    inline int skip(std::initializer_list<int> list);
};



inline int AbstractInput::approach(int ch) {
    while (available()) {
        if (forward() == ch) {
            return ch;
        }
        read();
    }
    return -1;
}

inline int AbstractInput::approach(int ch1, int ch2) {
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

inline int AbstractInput::approach(int ch1, int ch2, int ch3) {
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

inline void prepareBitClazz(bool* clazz, std::initializer_list<int>& list) {
    memset(clazz, false, 128);
    for (const auto &ch : list) {
        if (ch >= 0 && ch < 128) {
            clazz[ch] = true;
        }
    }
}

inline int AbstractInput::approach(std::initializer_list<int> list) {
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

inline int AbstractInput::find(int ch) {
    while (available()) {
        if (read() == ch) {
            return ch;
        }
    }
    return -1;
}

inline int AbstractInput::find(int ch1, int ch2) {
    while (available()) {
        int cur = read();
        if (cur == ch1 || cur == ch2) {
            return cur;
        }
    }
    return -1;
}

inline int AbstractInput::find(int ch1, int ch2, int ch3) {
    while (available()) {
        int cur = read();
        if (cur == ch1 || cur == ch2 || cur == ch3) {
            return cur;
        }
    }
    return -1;
}

inline int AbstractInput::find(std::initializer_list<int> list) {
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

inline int AbstractInput::skip(int ch) {
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

inline int AbstractInput::skip(int ch1, int ch2) {
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

inline int AbstractInput::skip(int ch1, int ch2, int ch3) {
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

inline int AbstractInput::skip(std::initializer_list<int> list) {
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

END_LR

#endif //ABSTRACTINPUT_H
