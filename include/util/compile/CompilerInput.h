//
// Created by stupid_coder_jyy on 2023/12/29.
//

#ifndef LIFERHYTHM_COMPILERINPUT_H
#define LIFERHYTHM_COMPILERINPUT_H

#include "BufferedInput.h"
#include <QVector>
#include "IByteReader.h"
#include "CompileError.h"
#include "ErrorHandler.h"

class CompilerInput : public BufferedInput{
private:
    static const int RESERVED_SIZE = 32;
    static const int MARK_LEN = 5;
    static int** reservedMarks;
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
    void fillA() override;
    void fillB() override;
private:
    int* getData();
    static int* offerData();
    void removeFirstData();
    int* removeLastData();

    CompileError pointError(const QString& msg, int pos);
    CompileError rangedError(const QString& msg, int end, int start);
};
#endif //LIFERHYTHM_COMPILERINPUT_H
