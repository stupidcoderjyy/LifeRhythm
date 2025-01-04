
#ifndef OPENRANGE_LEXER_H
#define OPENRANGE_LEXER_H

#include "Compiler.h"
#include "Namespaces.h"

BEGIN_LR
class CompilerInput;
END_NP

BEGIN_NP(lr::openrange)

class Lexer final : public DFALexer {
    friend class Parser;
public:
    Lexer();
};

class TokenNum final : public Token {
public:
    int val = 0;
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, AbstractInput *input) override;
};

END_NP

#endif