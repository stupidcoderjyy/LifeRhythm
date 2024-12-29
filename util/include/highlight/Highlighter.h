//
// Created by stupid_coder_jyy on 2024/2/13.
//

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include "Identifier.h"
#include "StringInput.h"
#include "Plugin.h"

BEGIN_NP(lr::highlight)

class CORE_API Context{};

class CORE_API Input : public StringInput{
public:
    int pos = 0;
public:
    explicit Input(const QString& blockText);
    int read() override;
    QString readUtf() override;
    void retractUtf(int count = 1);
    int retract() override;
    int retract(int count) override;
};

class CORE_API Token {
public:
    enum Op {
        Everything,
        NoEnd,
        NoFormat,
        NoEndAndFormat
    };
    int begin{};
    int end{};
    QTextCharFormat fmt{};
    virtual int type();
    virtual Op onMatched(const QString& lexeme, Input* input, Context* ctx);
    virtual ~Token() = default;
};

class CORE_API TokenSingle : public Token {
private:
    int ch{};
public:
    int type() override;
    Op onMatched(const QString &lexeme, Input *input, Context* ctx) override;
};

class CORE_API TokenError : public Token {
public:
    enum ErrorType {
        InvalidSymbol,
        InvalidEncoding
    };
    ErrorType errType;
public:
    explicit TokenError(ErrorType type);
    int type() override;
};

class CORE_API Lexer {
public:
    typedef std::function<Token*()> TokenSupplier;
public:
    QMap<QString, QTextCharFormat>* styleGroup{};
protected:
    QMap<int, QString> idToStyleName{};
    int statesCount;
    int startState;
    bool* accepted;
    int** goTo;
    TokenSupplier* tokens;
    Input* input{};
public:
    Lexer(const Identifier& styleGroupLoc, int statesCount, int startState);
    void reset(Input* in);
    virtual Token* run(Context* ctx) noexcept;
    virtual ~Lexer();
};

template<class CTX>
class CORE_API Highlighter : public QSyntaxHighlighter{
protected:
    Lexer* lexer;
    CTX* ctx;
public:
    explicit Highlighter(CTX* ctx, Lexer* lexer, QTextDocument *parent):
            QSyntaxHighlighter(parent),lexer(lexer),ctx(ctx){
    }
    ~Highlighter() override {
        delete lexer;
        delete ctx;
    }
};

END_NP

#endif //HIGHLIGHTER_H
