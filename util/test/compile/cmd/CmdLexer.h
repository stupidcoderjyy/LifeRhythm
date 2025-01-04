
#ifndef CMD_LEXER_H
#define CMD_LEXER_H

#include "Compiler.h"
#include "HighlightInput.h"
#include "Highlighter.h"
#include "HighlightLexer.h"

BEGIN_NP(lr::cmd)

enum Types {
    ID = 128,
    ARG = 129
};

class CmdLexer : public HighlightLexer {
public:
    explicit CmdLexer(Highlighter* highlighter);
};

class TokenArg final : public Token {
public:
    QString arg;
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, AbstractInput *input) override;
};

class TokenId final : public Token {
private:
    QString id;
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, AbstractInput *input) override;
};

END_NP

#endif