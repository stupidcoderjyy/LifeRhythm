
#include "Lexer.h"
#include "CompilerInput.h"

using namespace snbt;

Lexer::Lexer(CompilerInput *input): AbstractLexer(input, 15, 7){
}

void Lexer::init() {
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
}

Token::MatchResult TokenInt::onMatched(const QString &lexeme, CompilerInput *input) {
    data = lexeme.toInt();
    return Accept;
}

int TokenInt::type() {
    return 129;
}

Token::MatchResult TokenFloat::onMatched(const QString &lexeme, CompilerInput *input) {
    if (lexeme.endsWith('f') || lexeme.endsWith('F')) {
        data = lexeme.mid(0, lexeme.length() -1).toFloat();
    } else {
        data = lexeme.toFloat();
    }
    return Accept;
}

int TokenFloat::type() {
    return 130;
}

Token::MatchResult TokenComment::onMatched(const QString &lexeme, CompilerInput *input) {
    if (lexeme[1] == '/') {
        input->find('\n');
    } else {
        while (true) {
            if (input->find('*') < 0) {
                return Error;
            }
            if (!input->available()) {
                return Error;
            }
            if (input->read() == '/') {
                break;
            }
        }
    }
    return Ignore;
}

int TokenComment::type() {
    return 0;
}

Token::MatchResult TokenString::onMatched(const QString &lexeme, CompilerInput *input) {
    data = lexeme.mid(1, lexeme.length() - 2);
    return Accept;
}

int TokenString::type() {
    return 128;
}

Token::MatchResult TokenId::onMatched(const QString &lexeme, CompilerInput *input) {
    data = lexeme;
    return Accept;
}

int TokenId::type() {
    return keyWords.value(data, 133);
}

QMap<QString, int> initMap() {
    QMap<QString, int> map{};
    map.insert("false", 132);
    map.insert("true", 131);
    return map;
}

QMap<QString, int> TokenId::keyWords = initMap();
