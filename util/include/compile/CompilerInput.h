//
// Created by stupid_coder_jyy on 2023/12/29.
//

#ifndef COMPILERINPUT_H
#define COMPILERINPUT_H

#include "BufferedInput.h"
#include <QVector>
#include "IByteReader.h"
#include "CompileError.h"

BEGIN_LR

class CORE_API CompilerInput : public BufferedInput{
protected:
    static constexpr int RESERVED_SIZE = 16;
    static constexpr int MARK_LEN = 8;
    static QVector<int*> removed;
    static int mPos;
    int row = 1;
    int column = 0;
    const QString filePath;
    QVector<int> columnSizes{};
    QVector<int> rowBegin{};
    QVector<int*> markData{};
public:
    CompilerInput(IByteReader* reader, QString filePath, int bufSize = DEFAULT_BUFFER_SIZE);
    ~CompilerInput() override;
    static CompilerInput* fromFile(const QString& file);
    static CompilerInput* fromFile(const QString& file, int bufSize);
    static CompilerInput* fromString(const QString& str);
    void mark() override;
    void removeMark() override;
    int retract() override;
    int retract(int count) override;
    int read() override;
    QString currentLine();
    void recover(bool consume) override;
    CompileError errorAtMark(const QString& msg);
    CompileError errorAtForward(const QString& msg);
    CompileError errorMarkToMark(const QString& msg);
    CompileError errorMarkToForward(const QString& msg);
protected:
    static int* offerData();
    void fillA() override;
    void fillB() override;
    int* getData();
    void removeFirstData();
    int* removeLastData();
    virtual void recoverFromData(int* data);
    CompileError pointError(const QString& msg, int pos);
    CompileError rangedError(const QString& msg, int end, int start);
};

END_LR

#endif //COMPILERINPUT_H
