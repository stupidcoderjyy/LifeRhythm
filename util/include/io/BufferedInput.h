//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef BUFFEREDINPUT_H
#define BUFFEREDINPUT_H
#include <QVector>
#include "IByteReader.h"
#include "AbstractInput.h"

BEGIN_LR

class CORE_API BufferedInput : public AbstractInput{
protected:
    static constexpr int DEFAULT_BUFFER_SIZE = 2048;
    static constexpr int MAX_BUFFER_SIZE = 4096;
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
    ~BufferedInput() override;
protected:
    virtual void fillA();
    virtual void fillB();
    QString capture(int end, int start) const;
private:
    void _fillA();
    void _mark0();
};

END_LR

#endif //BUFFEREDINPUT_H
