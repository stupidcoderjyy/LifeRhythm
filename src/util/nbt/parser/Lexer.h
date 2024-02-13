
#ifndef LIFERHYTHM_LEXER_H
#define LIFERHYTHM_LEXER_H

#include "Compiler.h"
#include "MemUtil.h"
#include <QMap>

class CompilerInput;

namespace snbt {
    class Lexer : public AbstractLexer{
        friend class Parser;
    public:
        explicit Lexer(CompilerInput* input);
        void init() override;
    };

    class TokenInt : public Token{
    public:
        int data;
    public:
        int type() override;
        MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
    };

    class TokenFloat : public Token{
    public:
        float data{};
    public:
        int type() override;
        MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
    };

    class TokenComment : public Token{
    public:
        int type() override;
        MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
    };

    class TokenString : public Token{
    public:
        QString data{};
    public:
        int type() override;
        MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
    };

    class TokenId : public Token{
    private:
        static QMap<QString, int> keyWords;
    public:
        QString data{};
    public:
        int type() override;
        MatchResult onMatched(const QString &lexeme, CompilerInput *input) override;
    };
}
#endif