//
// Created by JYY on 25-1-3.
//

#include "CmdHighlighter.h"

#include "Styles.h"

USING_NP(lr::cmd)

CmdHighlighter::CmdHighlighter(const Identifier &styleGroup, QTextDocument *doc): Highlighter(styleGroup, doc, new CmdLexer(this)) {
}

void CmdHighlighter::prepareStyles(HighlightInput &input) {
    auto token = lexer->nextToken(&input);
    if (token == TokenFileEnd::get()) {
        return;
    }
    if (!token) {
        push(Styles::FORMAT_ERROR);
        flush();
        return;
    }
    switch (token->type()) {
        case ID: {
            push("id");
            break;
        }
        case ARG: {
            push(Styles::FORMAT_DEFAULT, lexer->begin(), lexer->begin() + 1);
            push("arg", lexer->begin() + 1, lexer->end());
            break;
        }
        default: {
            push(Styles::FORMAT_DEFAULT);
        }
    }
    flush();
}
