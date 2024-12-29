
#ifndef OPENRANGE_LEXER_H
#define OPENRANGE_LEXER_H

#include "Compiler.h"
#include "Namespaces.h"

BEGIN_LR
class CompilerInput;
END_LR

BEGIN_NP(lr::openrange)

class Lexer final : public AbstractLexer {
    friend class SyntaxAnalyzer;
public:
    explicit Lexer(CompilerInput* input);
};

class TokenNum final : public Token{
public:
    int val = 0;
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
};

END_NP

#endif