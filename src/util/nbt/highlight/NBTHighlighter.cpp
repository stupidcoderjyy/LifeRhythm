//
// Created by stupid_coder_jyy on 2024/2/14.
//

#include "NBTHighlighter.h"
#include "Styles.h"
#include "QTextDocument"

using namespace highlight;

NBTLexer::NBTLexer():Lexer("lr:nbt", 15, 7) {
    goTo[4][95] = 4;
    goTo[5][70] = 12;
    goTo[5][102] = 12;
    goTo[6][70] = 12;
    goTo[6][102] = 12;
    goTo[6][46] = 9;
    goTo[7][46] = 9;
    goTo[6][76] = 11;
    goTo[6][108] = 11;
    goTo[7][34] = 8;
    goTo[7][44] = 1;
    goTo[7][58] = 1;
    goTo[7][91] = 1;
    goTo[7][93] = 1;
    goTo[7][123] = 1;
    goTo[7][125] = 1;
    goTo[7][47] = 10;
    goTo[8][34] = 2;
    goTo[8][92] = 14;
    goTo[10][42] = 3;
    goTo[10][47] = 3;
    goTo[13][34] = 2;
    goTo[13][92] = 14;
    goTo[14][92] = 14;
    goTo[14][34] = 13;
    for(int i = 0 ; i <= 33 ; i ++) {
        goTo[8][i] = 8;
        goTo[13][i] = 8;
        goTo[14][i] = 8;
    }
    for(int i = 48 ; i <= 57 ; i ++) {
        goTo[4][i] = 4;
        goTo[5][i] = 5;
        goTo[9][i] = 5;
        goTo[6][i] = 6;
        goTo[7][i] = 6;
    }
    for(int i = 97 ; i <= 122 ; i ++) {
        goTo[4][i] = 4;
        goTo[7][i] = 4;
    }
    for(int i = 65 ; i <= 90 ; i ++) {
        goTo[4][i] = 4;
        goTo[7][i] = 4;
    }
    for(int i = 93 ; i <= 127 ; i ++) {
        goTo[8][i] = 8;
        goTo[13][i] = 8;
        goTo[14][i] = 8;
    }
    for(int i = 35 ; i <= 91 ; i ++) {
        goTo[8][i] = 8;
        goTo[13][i] = 8;
        goTo[14][i] = 8;
    }

    for(int i = 1 ; i <= 6 ; i ++) {
        accepted[i] = true;
    }
    for(int i = 11 ; i <= 13 ; i ++) {
        accepted[i] = true;
    }

    TokenSupplier e1 = []() {return (new TokenString());};
    TokenSupplier e4 = []() {return (new TokenFloat());};
    TokenSupplier e5 = []() {return (new TokenInt());};
    tokens[1] = []() {return (new TokenSingle());};
    tokens[2] = e1;
    tokens[13] = e1;
    tokens[3] = []() {return (new TokenComment());};
    tokens[4] = []() {return (new TokenId());};
    tokens[5] = e4;
    tokens[12] = e4;
    tokens[6] = e5;
    tokens[11] = e5;

    idToStyleName.insert(128, "string");
    idToStyleName.insert(129, "num");
    idToStyleName.insert(130, "num");
    idToStyleName.insert(-2, "comment");
    idToStyleName.insert('[', "arr");
    idToStyleName.insert(']', "arr");
    idToStyleName.insert(',', "arr");
    idToStyleName.insert(131, "bool");
    idToStyleName.insert(132, "bool");
}

Token::Op TokenInt::onMatched(const QString &lexeme, Input *input, Context* ctx) {
    data = lexeme.toInt();
    return Everything;
}

int TokenInt::type() {
    return 129;
}

Token::Op TokenFloat::onMatched(const QString &lexeme, Input *input, Context* ctx) {
    if (lexeme.endsWith('f') || lexeme.endsWith('F')) {
        data = lexeme.midRef(0, lexeme.length() -1).toFloat();
    } else {
        data = lexeme.toFloat();
    }
    return Everything;
}

int TokenFloat::type() {
    return 130;
}

Token::Op TokenComment::onMatched(const QString &lexeme, Input *input, Context* ctx) {
    auto* nbtCtx = static_cast<NBTContext*>(ctx);
    if (lexeme[1] == '*') {
        if (nbtCtx->highlighter->previousBlockState() == 1) {
            return Everything;
        }
        if (input->find('*') > 0 && input->available() && input->read() == '/') {
            end = input->pos;
            return NoEnd;
        }
        nbtCtx->highlighter->setCurrentBlockState(1);
    }
    while (input->available()) {
        input->read();
    }
    end = input->pos;
    return NoEnd;
}

int TokenComment::type() {
    return -2;
}

Token::Op TokenString::onMatched(const QString &lexeme, Input *input, Context* ctx) {
    data = lexeme.mid(1, lexeme.length() - 2);
    return Everything;
}

int TokenString::type() {
    return 128;
}

Token::Op TokenId::onMatched(const QString &lexeme, Input *input, Context* ctx) {
    data = lexeme;
    return Everything;
}

int TokenId::type() {
    return keyWords.value(data, 133);
}

QMap<QString, int> TokenId::initMap() {
    QMap<QString, int> map{};
    map.insert("false", 132);
    map.insert("true", 131);
    return map;
}

QMap<QString, int> TokenId::keyWords = TokenId::initMap();

NBTHighlighter::NBTHighlighter(NBTContext *ctx, Lexer *lexer, QTextDocument *parent):
        Highlighter(ctx, lexer,parent) {
    ctx->highlighter = this;
}

void NBTHighlighter::highlightBlock(const QString &text) {
    return;
    setCurrentBlockState(0);
    if (ctx->globalFailure) {
        setFormat(0, text.length(), Styles::FORMAT_ERROR);
        return;
    }
    Input input(text);
    if (previousBlockState() == 1) {    //上一行是多行注释
        if (input.find('*') > 0 && input.available() && input.read() == '/') {
            setFormat(0, input.pos, lexer->styleGroup->value("comment"));
        } else {
            setFormat(0, text.length(), lexer->styleGroup->value("comment"));
            setCurrentBlockState(1);   //多行注释继续
            return;
        }
    }
    lexer->reset(&input);
    while (input.available()) {
        Token* token = lexer->run(ctx);
        if (!token) {
            continue;
        }
        if (token->type() != -1) {  //error
            setFormat(token->begin, token->end - token->begin, token->fmt);
            delete token;
            continue;
        }
        auto* err = static_cast<TokenError*>(token);
        if (err->errType == TokenError::InvalidSymbol) {
            setFormat(token->begin, token->end - token->begin, Styles::FORMAT_ERROR);
            delete token;
        } else {
            setFormat(0, text.length(), Styles::FORMAT_ERROR);
            ctx->globalFailure = true;
            delete token;
            return;
        }
    }
}
