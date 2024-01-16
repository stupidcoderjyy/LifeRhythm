
#ifndef LIFERHYTHM_STRINGNBTLEXER_H
#define LIFERHYTHM_STRINGNBTLEXER_H

#include "Compiler.h"
#include "MemUtil.h"

class CompilerInput;

class StringNbtLexer {
    friend class StringNbtParser;
public:
    typedef std::function<Token*(const QString&, CompilerInput*)> TokenSupplier;
private:
    bool* accepted;
    int** goTo{};
    TokenSupplier* tokens;
    CompilerInput* input;
public:
    explicit StringNbtLexer(CompilerInput* input);
    Token* run();
    virtual ~StringNbtLexer();
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

class TokenId : public Token{
private:
    static QMap<QString, int> keyWords;
public:
    QString data{};
public:
    int type() override {
        return keyWords.value(data, 133);
    }
    Token *onMatched(const QString &lexeme, CompilerInput *input) override {
        data = lexeme;
        return this;
    }
private:
    static QMap<QString, int> init() {
        QMap<QString, int> map{};
        map.insert("false", 132);
        map.insert("true", 131);
        return map;
    }
};
#endif