
#ifndef LIFERHYTHM_LEXER_H
#define LIFERHYTHM_LEXER_H

#include "Compiler.h"
#include "Namespaces.h"

class CompilerInput;

BEGIN_NAMESPACE(lr::openrange)

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

END_NAMESPACE

#endif