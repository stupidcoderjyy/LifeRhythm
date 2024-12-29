//
// Created by stupid_coder_jyy on 2023/12/29.
//

#ifndef COMPILEERROR_H
#define COMPILEERROR_H

#include <QString>
#include "ErrorHandler.h"
#include "Namespaces.h"
#include "Plugin.h"

BEGIN_LR

class CORE_API CompileError : public std::exception{
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

class CORE_API PrintCompileErrorHandler : public ErrorHandler<CompileError> {
public:
    void onErrorCaught(CompileError &err) override;
};

END_LR

#endif //COMPILEERROR_H
