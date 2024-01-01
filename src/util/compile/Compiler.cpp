//
// Created by stupid_coder_jyy on 2023/12/30.
//
#include "Compiler.h"

TokenFileEnd *TokenFileEnd::get() {
    static auto* instance = new TokenFileEnd();
    return instance;
}

int TokenFileEnd::type() {
    return 0;
}

Token *TokenFileEnd::onMatched(const QString &lexeme, CompilerInput *input) {
    return this;
}

int TokenSingle::type() {
    return ch;
}

Token *TokenSingle::onMatched(const QString &lexeme, CompilerInput *input) {
    ch = lexeme.at(0).cell();
    return this;
}