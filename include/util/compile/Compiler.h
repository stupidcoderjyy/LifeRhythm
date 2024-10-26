//
// Created by stupid_coder_jyy on 2023/12/30.
//

#ifndef LIFERHYTHM_COMPILER_H
#define LIFERHYTHM_COMPILER_H
#include <QString>
#include "CompileError.h"
#include "Plugin.h"
#include <QVector>

class CompilerInput;

class CORE_API Token{
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

class CORE_API TokenFileEnd final : public Token{
public:
    static TokenFileEnd* get();
    int type() override;
    MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
private:
    TokenFileEnd() = default;
};

class CORE_API TokenSingle final : public Token{
private:
    uchar ch{};
public:
    int type() override;
    MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
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

class CORE_API Property{
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

class CORE_API AbstractLexer {
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
    virtual Token* run() noexcept;
    virtual ~AbstractLexer();
};

class CORE_API AbstractSyntaxAnalyzer {
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
    void run();
    virtual ~AbstractSyntaxAnalyzer();
protected:
    virtual void onFinished();
    virtual void onFailed();
    virtual void onReduced();
    virtual void onShifted();
};

#endif //LIFERHYTHM_COMPILER_H
