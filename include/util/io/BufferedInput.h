//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_BUFFEREDINPUT_H
#define LIFERHYTHM_BUFFEREDINPUT_H
#include <QVector>
#include "IByteReader.h"
#include "AbstractInput.h"
#include <initializer_list>

class BufferedInput : public AbstractInput{
protected:
    const static int DEFAULT_BUFFER_SIZE = 2048;
    const static int MAX_BUFFER_SIZE = 4096;
    IByteReader* reader;
    QVector<int> marks{};
    char* buffer{};
    int bufEndA = 0;
    int bufEndB = 0;
    int inputEnd = -1;
    int next = 0;
    int fillCount = 0;
public:
    BufferedInput(IByteReader* reader, int bufSize);
    explicit BufferedInput(IByteReader* reader);
    static BufferedInput* fromFile(const QString& file);
    static BufferedInput* fromFile(const QString& file, int bufSize);
    static BufferedInput* fromString(const QString& str);
    void mark() override;
    void removeMark() override;
    void recover(bool consume) override;
    void recover() override;
    int retract() override;
    int retract(int count) override;
    bool available() const override;
    int read() override;
    int forward() override;
    QString capture() override;
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
