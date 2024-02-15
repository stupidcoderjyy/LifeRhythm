//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_BUFFEREDINPUT_H
#define LIFERHYTHM_BUFFEREDINPUT_H
#include <QVector>
#include "IByteReader.h"
#include <initializer_list>

class BufferedInput {
protected:
    const static int DEFAULT_BUFFER_SIZE = 2048;
    const static int MAX_BUFFER_SIZE = 4096;
    IByteReader* reader;
    QVector<int> marks{};
    char* buffer{};
    char* bitClazz{};
    int bufEndA = 0;
    int bufEndB = 0;
    int inputEnd = -1;
    int forward = 0;
    int fillCount = 0;
public:
    BufferedInput(IByteReader* reader, int bufSize);
    explicit BufferedInput(IByteReader* reader);
    static BufferedInput* fromFile(const QString& file);
    static BufferedInput* fromFile(const QString& file, int bufSize);
    static BufferedInput* fromString(const QString& str);

    virtual void mark();
    virtual void removeMark();
    virtual void recover(bool consume);
    void recover();
    virtual int retract();
    virtual int retract(int count);
    bool available() const;
    virtual int read();
    virtual QString readUtf();
    QString capture();
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
    virtual ~BufferedInput();
protected:
    virtual void fillA();
    virtual void fillB();
    QString capture(int end, int start);
private:
    void _fillA();
    void _mark0();
};

#endif //LIFERHYTHM_BUFFEREDINPUT_H
