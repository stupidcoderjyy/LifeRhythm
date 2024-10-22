//
// Created by stupid_coder_jyy on 2024/2/14.
//

#ifndef LIFERHYTHM_NBTHIGHLIGHTER_H
#define LIFERHYTHM_NBTHIGHLIGHTER_H

#include "Highlighter.h"

namespace highlight{

class NBTHighlighter;

class CORE_API NBTContext : public Context{
public:
    bool globalFailure = false;
    NBTHighlighter* highlighter = nullptr;
};

class CORE_API NBTHighlighter : public Highlighter<NBTContext>{
    friend class TokenComment;
public:
    NBTHighlighter(NBTContext *ctx, Lexer *lexer, QTextDocument *parent);
protected:
    void highlightBlock(const QString &text) override;
};

class CORE_API NBTLexer : public Lexer{
public:
    NBTLexer();
};

class CORE_API TokenInt : public Token{
public:
    int data{};
public:
    int type() override;
    Op onMatched(const QString &lexeme, Input *input, Context* ctx) override;
};

class CORE_API TokenFloat : public Token{
public:
    float data{};
public:
    int type() override;
    Op onMatched(const QString &lexeme, Input *input, Context* ctx) override;
};

class CORE_API TokenComment : public Token{
public:
    int type() override;
    Op onMatched(const QString &lexeme, Input *input, Context* ctx) override;
};

class CORE_API TokenString : public Token{
public:
    QString data{};
public:
    int type() override;
    Op onMatched(const QString &lexeme, Input *input, Context* ctx) override;
};

class CORE_API TokenId : public Token{
private:
    static QMap<QString, int> keyWords;
public:
    QString data{};
public:
    int type() override;
    Op onMatched(const QString &lexeme, Input *input, Context* ctx) override;
private:
    static QMap<QString, int> initMap();
};

}


#endif //LIFERHYTHM_NBTHIGHLIGHTER_H
