
#ifndef SNBT_LEXER_H
#define SNBT_LEXER_H

#include "Compiler.h"
#include <QMap>

#include "Namespaces.h"

class CompilerInput;

BEGIN_NP(lr::snbt)

class CORE_API Lexer : public AbstractLexer{
    friend class Parser;
public:
    explicit Lexer(CompilerInput* input);
};

class CORE_API TokenInt : public Token{
public:
    long long data;
    bool isLong;
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
};

class CORE_API TokenFloat : public Token{
public:
    float data{};
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
};

class CORE_API TokenComment : public Token{
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
};

class CORE_API TokenString : public Token{
public:
    QString data{};
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
};

class CORE_API TokenId : public Token{
private:
    static QMap<QString, int> keyWords;
public:
    QString data{};
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
private:
    static QMap<QString, int> initMap();
};

END_NP

#endif