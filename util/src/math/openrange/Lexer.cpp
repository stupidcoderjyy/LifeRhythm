
#include "Lexer.h"
#include "CompilerInput.h"

#define STATES_COUNT 5
#define START_STATE 3

USING_NP(lr::openrange)

Lexer::Lexer(CompilerInput *input): AbstractLexer(input, STATES_COUNT, START_STATE) {
    goTo[3][40] = 1;
    goTo[3][41] = 1;
    goTo[3][44] = 1;
    goTo[3][85] = 1;
    goTo[3][91] = 1;
    goTo[3][93] = 1;
    goTo[3][123] = 1;
    goTo[3][125] = 1;
    goTo[3][43] = 4;
    goTo[3][45] = 4;
    for(int i = 48 ; i <= 57 ; i ++) {
        goTo[2][i] = 2;
        goTo[3][i] = 2;
        goTo[4][i] = 2;
    }

    accepted[1] = true;
    accepted[2] = true;

    tokens[1] = []{return new TokenSingle();};
    tokens[2] = []{return new TokenNum();};
}

lr::Token::MatchResult TokenNum::onMatched(const QString &lexeme, CompilerInput *input) {
    val = lexeme.toInt();
    return Accept;
}

int TokenNum::type() {
    return 128;
}

