//
// Created by stupid_coder_jyy on 2023/12/30.
//

#ifndef COMPILER_H
#define COMPILER_H
#include <QString>
#include "CompileError.h"
#include "Plugin.h"
#include <QVector>

BEGIN_LR

class AbstractInput;

class CORE_API Token {
public:
    enum MatchResult {
        Accept,
        Ignore,
        Error
    };
public:
    virtual int type() = 0;
    virtual MatchResult onMatched(const QString& lexeme, AbstractInput* input) = 0;
    virtual ~Token() = default;
    template<class T> T* cast() {
        return static_cast<T*>(this);
    }
};

class CORE_API TokenFileEnd final : public Token{
public:
    static TokenFileEnd* get();
    int type() override;
    MatchResult onMatched(const QString &lexeme, AbstractInput* input) override;
private:
    TokenFileEnd() = default;
};

class CORE_API TokenSingle final : public Token{
private:
    uchar ch{};
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, AbstractInput* input) override;
};

class CORE_API Symbol{
public:
    bool isTerminal;
    int id;
public:
    Symbol(bool isTerminal, int id): isTerminal(isTerminal), id(id) {}
};

class CORE_API Production final {
public:
    int id;
    int bodyLen;
    Symbol* head;
    Symbol** body;

public:
    Production(int id, Symbol *head, int bodyLen, Symbol **body): id(id), bodyLen(bodyLen),head(head), body(body) {}
    ~Production() {
        delete[] body;
    }
};

class CORE_API Property {
public:
    virtual void onReduced(Production* p, Property** properties) = 0;
    virtual ~Property() = default;
};

class CORE_API PropertyTerminal final : public Property{
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

class CORE_API ILexer {
public:
    virtual Token *nextToken(AbstractInput *input) noexcept;
    virtual ~ILexer() = default;
};

class CORE_API DFALexer : public ILexer {
public:
    typedef std::function<Token*()> TokenSupplier;
protected:
    int statesCount;
    int startState;
    bool* accepted;
    int** goTo;
    TokenSupplier* tokens;
public:
    DFALexer(int statesCount, int startState);
    Token *nextToken(AbstractInput* input) noexcept override;
    ~DFALexer() override;
};

class CORE_API LALRParser {
public:
    typedef std::function<Property*()> PropertySupplier;
protected:
    int statesCount;
    int** actions;
    int** goTo;
    int* terminalRemap;
    QVector<Production*> productions{};
    PropertySupplier* suppliers;
    ILexer* lexer;
    QVector<Symbol*> symbols{};
    AbstractInput *input;
public:
    LALRParser(ILexer* lexer, int remap, int nonTerminal, int terminal, int states);
    void run(AbstractInput* input);
    virtual ~LALRParser();
protected:
    virtual void onFinished();
    virtual void onFailed(Token *at);
    virtual void onReduced();
    virtual void onShifted();
};

END_NP

#endif //COMPILER_H
