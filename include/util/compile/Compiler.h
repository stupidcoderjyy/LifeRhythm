//
// Created by stupid_coder_jyy on 2023/12/30.
//

#ifndef LIFERHYTHM_COMPILER_H
#define LIFERHYTHM_COMPILER_H
#include <QString>
#include "CompileError.h"
#include <QVector>

class CompilerInput;

class Token{
public:
    enum MatchResult {
        Accept,
        Ignore,
        Error
    };
    virtual int type() = 0;
    virtual MatchResult onMatched(const QString& lexeme, CompilerInput* input) = 0;
    virtual ~Token() = default;
};

class TokenFileEnd : public Token{
public:
    static TokenFileEnd* get();
    int type() override;
    MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
private:
    TokenFileEnd() = default;
};

class TokenSingle : public Token{
private:
    uchar ch{};
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
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

class AbstractLexer {
    friend class AbstractSyntaxAnalyzer;
public:
    typedef std::function<Token*()> TokenSupplier;
protected:
    int statesCount;
    int startState;
    bool* accepted;
    int** goTo;
    TokenSupplier* tokens;
    CompilerInput* input;
public:
    AbstractLexer(CompilerInput* input, int statesCount, int startState);
    virtual void init() = 0;
    virtual Token* run();
    virtual ~AbstractLexer();
};

class AbstractSyntaxAnalyzer {
public:
    typedef std::function<Property*()> PropertySupplier;
protected:
    int statesCount;
    int** actions;
    int** goTo;
    int* terminalRemap;
    QVector<Production*> productions{};
    PropertySupplier* suppliers;
    AbstractLexer* lexer;
    CompilerInput* input;
    QVector<Symbol*> symbols{};
public:
    AbstractSyntaxAnalyzer(AbstractLexer* lexer, int remap, int nonTerminal, int terminal, int states);
    void init();
    void run();
    virtual ~AbstractSyntaxAnalyzer();
protected:
    virtual void initActions() = 0;
    virtual void initGoTo() = 0;
    virtual void initOthers() = 0;
    virtual void initGrammar() = 0;
    virtual void onFinished();
    virtual void onFailed();
    virtual void onReduced();
    virtual void onShifted();
};

#endif //LIFERHYTHM_COMPILER_H
