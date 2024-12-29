//
// Created by stupid_coder_jyy on 2023/12/29.
//

#include "CompileError.h"
#include <utility>
#include "iostream"
#include "IOUtil.h"

USING_LR

CompileError::CompileError(QString msg, int row, QString line, QString filePath):
        row(row),line(std::move(line)),msg(std::move(msg)),filePath(std::move(filePath)) {

}

CompileError &CompileError::setPos(int column) {
    start = column;
    end = column;
    return *this;
}

CompileError &CompileError::setPos(int _start, int _end) {
    this->start = _start;
    this->end = _end;
    return *this;
}

void CompileError::printErr() {
    setConsoleColor(CONSOLE_RED);
    std::cout << filePath.toStdString();
    std::cout << ":" << row << ":" << start << ":" << msg.toStdString() << std::endl;
    std::cout << "    " << line.toStdString() << std::endl;
    std::cout << QString(" ").repeated(start + 4).toStdString();
    std::cout << QString("^").repeated(end - start + 1).toStdString() << std::endl;
}

void PrintCompileErrorHandler::onErrorCaught(CompileError &err) {
    err.printErr();
}

CompileError::CompileError(const CompileError &o):
    line(o.line),
    msg(o.msg),
    filePath(o.filePath),
    row(o.row),
    start(o.start),
    end(o.end){

}

CompileError::CompileError(CompileError &&o) noexcept :
        line(o.line),
        msg(o.msg),
        filePath(o.filePath),
        row(o.row),
        start(o.start),
        end(o.end){
}
