
#include "CmdLexer.h"
#include "CompilerInput.h"
#include "Error.h"
#include "QDebug"
#include "Styles.h"

USING_NP(lr::cmd)
using lr::Token;

CmdLexer::CmdLexer(Highlighter *h): HighlightLexer(h, 5, 4) {
    goTo[3][95] = 3;
    goTo[4][45] = 2;
    goTo[4][46] = 1;
    for (int i = 48; i <= 57; i++) {
        goTo[3][i] = 3;
    }
    for (int i = 97; i <= 122; i++) {
        goTo[3][i] = 3;
        goTo[4][i] = 3;
    }
    for (int i = 65; i <= 90; i++) {
        goTo[3][i] = 3;
        goTo[4][i] = 3;
    }

    for (int i = 1; i <= 3; i++) {
        accepted[i] = true;
    }

    tokens[1] = [] { return new TokenSingle(); };
    tokens[2] = [] { return new TokenArg(); };
    tokens[3] = [] { return new TokenId(); };
}

Token::MatchResult TokenArg::onMatched(const QString &lexeme, AbstractInput *input) {
    input->mark();
    while (input->available()) {
        if (input->forward() == ' ') {
            break;
        }
        input->readUtf();
    }
    input->mark();
    arg = input->capture();
    return Accept;
}

int TokenArg::type() {
    return ARG;
}

Token::MatchResult TokenId::onMatched(const QString &lexeme, AbstractInput *input) {
    id = lexeme;
    return Accept;
}

int TokenId::type() {
    return ID;
}