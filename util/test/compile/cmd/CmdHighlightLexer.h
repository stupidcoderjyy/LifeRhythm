
#ifndef CMD_LEXER_H
#define CMD_LEXER_H

#include "Compiler.h"
#include "HighlightInput.h"
#include "Highlighter.h"
#include "HighlightLexer.h"

BEGIN_NP(lr::cmd::highlight)

enum Types {
    ID = 128,
    ARG = 129
};

class CmdHighlightLexer : public HighlightLexer {
public:
    explicit CmdHighlightLexer(Highlighter* highlighter);
};

class TokenSingleCmd final : public TokenSingle {
private:
    Highlighter *highlighter;
public:
    explicit TokenSingleCmd(Highlighter *highlighter);
    MatchResult onMatched(const QString &lexeme, AbstractInput *input) override;
};

class TokenArg final : public Token {
private:
    QString arg;
    Highlighter* highlighter;
public:
    explicit TokenArg(Highlighter* highlighter);
    int type() override;
    MatchResult onMatched(const QString &lexeme, AbstractInput *input) override;
    QString getArg() { return arg; }
};

class TokenId final : public Token {
private:
    QString id;
    Highlighter *highlighter;
public:
    explicit TokenId(Highlighter *highlighter);
    int type() override;
    MatchResult onMatched(const QString &lexeme, AbstractInput *input) override;
    QString getId() { return id; }
};

END_NP

#endif