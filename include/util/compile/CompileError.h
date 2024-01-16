//
// Created by stupid_coder_jyy on 2023/12/29.
//

#ifndef LIFERHYTHM_COMPILEERROR_H
#define LIFERHYTHM_COMPILEERROR_H

#include <QString>
#include "ErrorHandler.h"

class CompileError : public std::exception{
public:
    const int row;
    const QString line;
    const QString msg;
    const QString filePath;
    int start{};
    int end{};

    CompileError(QString msg, int row, QString line, QString filePath);
    CompileError& setPos(int column);
    CompileError& setPos(int _start, int _end);
    void printErr();

    CompileError(const CompileError& o);
    CompileError(CompileError&& o) noexcept ;
};

class PrintCompileErrorHandler : public ErrorHandler<CompileError> {
public:
    void onErrorCaught(CompileError &err) override;
};
#endif //LIFERHYTHM_COMPILEERROR_H
