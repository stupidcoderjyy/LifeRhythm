
#include "StringNbtLexer.h"
#include "MemUtil.h"
#include "CompilerInput.h"

#define statesCount 13

StringNbtLexer::StringNbtLexer(CompilerInput *input):
        accepted(new bool[statesCount]),
        tokens(new TokenSupplier[statesCount]),
        input(input){
    goTo = allocateArray(statesCount, 128);
    init();
}

void StringNbtLexer::init() {
    goTo[3][95] = 3;
    goTo[5][46] = 8;
    goTo[6][46] = 8;
    goTo[5][70] = 4;
    goTo[5][102] = 4;
    goTo[5][76] = 9;
    goTo[5][108] = 9;
    goTo[6][34] = 7;
    goTo[6][44] = 1;
    goTo[6][58] = 1;
    goTo[6][91] = 1;
    goTo[6][93] = 1;
    goTo[6][123] = 1;
    goTo[6][125] = 1;
    goTo[10][70] = 4;
    goTo[10][102] = 4;
    goTo[7][34] = 2;
    goTo[7][92] = 12;
    goTo[11][34] = 2;
    goTo[11][92] = 12;
    goTo[12][92] = 12;
    goTo[12][34] = 11;
    for(int i = 0 ; i <= 33 ; i ++) {
        goTo[7][i] = 7;
        goTo[11][i] = 7;
        goTo[12][i] = 7;
    }
    for(int i = 48 ; i <= 57 ; i ++) {
        goTo[3][i] = 3;
        goTo[5][i] = 5;
        goTo[6][i] = 5;
        goTo[8][i] = 10;
        goTo[10][i] = 10;
    }
    for(int i = 97 ; i <= 122 ; i ++) {
        goTo[3][i] = 3;
        goTo[6][i] = 3;
    }
    for(int i = 65 ; i <= 90 ; i ++) {
        goTo[3][i] = 3;
        goTo[6][i] = 3;
    }
    for(int i = 93 ; i <= 127 ; i ++) {
        goTo[7][i] = 7;
        goTo[11][i] = 7;
        goTo[12][i] = 7;
    }
    for(int i = 35 ; i <= 91 ; i ++) {
        goTo[7][i] = 7;
        goTo[11][i] = 7;
        goTo[12][i] = 7;
    }

    for(int i = 9 ; i <= 11 ; i ++) {
        accepted[i] = true;
    }
    for(int i = 1 ; i <= 5 ; i ++) {
        accepted[i] = true;
    }

    TokenSupplier e1 = [](const QString& lexeme, CompilerInput* in) {return (new TokenString())->onMatched(lexeme, in);};
    TokenSupplier e3 = [](const QString& lexeme, CompilerInput* in) {return (new TokenFloat())->onMatched(lexeme, in);};
    TokenSupplier e4 = [](const QString& lexeme, CompilerInput* in) {return (new TokenInt())->onMatched(lexeme, in);};
    tokens[1] = [](const QString& lexeme, CompilerInput* in) {return (new TokenSingle())->onMatched(lexeme, in);};
    tokens[2] = e1;
    tokens[11] = e1;
    tokens[3] = [](const QString& lexeme, CompilerInput* in) {return (new TokenId())->onMatched(lexeme, in);};
    tokens[4] = e3;
    tokens[10] = e3;
    tokens[5] = e4;
    tokens[9] = e4;
}

Token *StringNbtLexer::run() {
    input->skip(' ', '\r', '\n');
    input->mark();
    if (!input->available()) {
        return TokenFileEnd::get();
    }
    int state = 6;
    int lastAccepted = -2;
    int extraLoadedBytes = 0;
    while (input->available()){
        int b = input->read();
        state = goTo[state][b];
        if (state == 0) {
            extraLoadedBytes++;
            break;
        }
        if (accepted[state]) {
            lastAccepted = state;
            extraLoadedBytes = 0;
        } else {
            extraLoadedBytes++;
        }
    }
    if (lastAccepted < 0 || !tokens[lastAccepted]) {
        input->approach('\r', ' ', '\t');
        return nullptr;
    }
    input->retract(extraLoadedBytes);
    input->mark();
    return tokens[lastAccepted](input->capture(), input);
}

StringNbtLexer::~StringNbtLexer() {
    delete[] accepted;
    delete[] tokens;
    freeArray(goTo, statesCount);
}

Token *TokenInt::onMatched(const QString &lexeme, CompilerInput *input) {
    data = lexeme.toInt();
    return this;
}

Token *TokenFloat::onMatched(const QString &lexeme, CompilerInput *input) {
    if (lexeme.endsWith('f') || lexeme.endsWith('F')) {
        data = lexeme.mid(0, lexeme.length() -1).toFloat();
    } else {
        data = lexeme.toFloat();
    }
    return this;
}

Token *TokenString::onMatched(const QString &lexeme, CompilerInput *input) {
    data = lexeme.mid(1, lexeme.length() - 2);
    return this;
}

QMap<QString, int> TokenId::keyWords = init();