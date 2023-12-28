//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_BUFFEREDINPUT_H
#define LIFERHYTHM_BUFFEREDINPUT_H
#include <QVector>
#include "IByteReader.h"
#include <initializer_list>

class BufferedInput {
private:
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

    void mark();
    void removeMark();
    int retract();
    int retract(int count);
    bool available() const;
    int read();
    QString capture();
    int approach(int ch);
    int approach(std::initializer_list<int> list);
    ~BufferedInput();
private:
    void fillA();
    void fillB();
    QString capture(int end, int start);
};

#endif //LIFERHYTHM_BUFFEREDINPUT_H
