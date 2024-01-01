
#ifndef LIFERHYTHM_LEXER_H
#define LIFERHYTHM_LEXER_H

#include "Compiler.h"
#include "MemUtil.h"

class CompilerInput;

class Lexer {
    friend class SyntaxAnalyzer;
public:
    typedef std::function<Token*(const QString&, CompilerInput*)> TokenSupplier;
private:
    bool* accepted;
    int** goTo{};
    TokenSupplier* tokens;
    CompilerInput* input;
public:
    explicit Lexer(CompilerInput* input);
    Token* run();
    virtual ~Lexer();
private:
    void init();
};

class TokenInt : public Token{
public:
    int data;
public:
    int type() override {
        return 129;
    }
    Token *onMatched(const QString &lexeme, CompilerInput *input) override;
};

class TokenId : public Token{
public:
    QString data{};
public:
    int type() override {
        return 131;
    }
    Token *onMatched(const QString &lexeme, CompilerInput *input) override;
};

class TokenFloat : public Token{
public:
    float data{};
public:
    int type() override {
        return 130;
    }
    Token *onMatched(const QString &lexeme, CompilerInput *input) override;
};

class TokenString : public Token{
public:
    QString data{};
public:
    int type() override {
        return 128;
    }
    Token *onMatched(const QString &lexeme, CompilerInput *input) override;
};

#endif