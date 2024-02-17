//
// Created by stupid_coder_jyy on 2024/2/17.
//

#ifndef LIFERHYTHM_ABSTRACTINPUT_H
#define LIFERHYTHM_ABSTRACTINPUT_H

#include <QString>

class AbstractInput {
private:
    bool bitClazz[128]{};
public:
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
    int approach(int ch);
    int approach(int ch1, int ch2);
    int approach(int ch1, int ch2, int ch3);
    int approach(std::initializer_list<int> list);
    int find(int ch);
    int find(int ch1, int ch2);
    int find(int ch1, int ch2, int ch3);
    int find(std::initializer_list<int> list);
    int skip(int ch);
    int skip(int ch1, int ch2);
    int skip(int ch1, int ch2, int ch3);
    int skip(std::initializer_list<int> list);
};

#endif //LIFERHYTHM_ABSTRACTINPUT_H
