//
// Created by stupid_coder_jyy on 2023/12/30.
//

#ifndef LIFERHYTHM_COMPILER_H
#define LIFERHYTHM_COMPILER_H
#include <QString>

class CompilerInput;

class Token{
public:
    virtual int type() = 0;
    virtual Token* onMatched(const QString& lexeme, CompilerInput* input) = 0;
    virtual ~Token() = default;
};

class TokenFileEnd : public Token{
public:
    static TokenFileEnd* get();
    int type() override;
    Token *onMatched(const QString &lexeme, CompilerInput *input) override;

private:
    TokenFileEnd() = default;
};

class TokenSingle : public Token{
private:
    uchar ch{};

public:
    int type() override;
    Token *onMatched(const QString &lexeme, CompilerInput *input) override;
};

class Symbol{
public:
    bool isTerminal;
    int id;

public:
    Symbol(bool isTerminal, int id): isTerminal(isTerminal), id(id) {};
};

class Production{
public:
    int id;
    int bodyLen;
    Symbol* head;
    Symbol** body;

public:
    Production(int id, Symbol *head, int bodyLen, Symbol **body) : id(id), head(head),bodyLen(bodyLen), body(body) {}
    virtual ~Production() {
        delete[] body;
    };
};

#include "QDebug"

class Property{
public:
    virtual void onReduced(Production* p, Property** properties) = 0;
    virtual ~Property() = default;
};


class PropertyTerminal : public Property{
private:
    Token* token;

public:
    explicit PropertyTerminal(Token* token): token(token){}

    template<class T> T* getToken() {
        return static_cast<T*>(token);
    }

    ~PropertyTerminal() override {
        delete token;
    }

    void onReduced(Production *p, Property **properties) override {
    }
};

#endif //LIFERHYTHM_COMPILER_H
