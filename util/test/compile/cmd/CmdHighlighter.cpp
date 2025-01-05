//
// Created by JYY on 25-1-3.
//

#include "CmdHighlighter.h"
#include "Helpers.h"
#include "Styles.h"

USING_NP(lr::cmd::highlight)

CmdHighlighter::CmdHighlighter(const Identifier &styleGroup, QTextDocument *doc, CmdManager *manager):
        Highlighter(styleGroup, doc, new CmdHighlightLexer(this)),
        empty(new ArgumentPredicate),
        parser(new CmdHighlightParser(this, manager)) {
}

void CmdHighlighter::registerPredicate(const Identifier &loc, ArgumentPredicate *predicate) {
    predicates.insert(loc, predicate);
}

ArgumentPredicate* CmdHighlighter::getPredicate(const Identifier &loc) const {
    return predicates.value(loc, empty);
}

void CmdHighlighter::highlightLex(HighlightInput &input) {
    while (input.available()) {
        delete lexer->nextToken(&input);
    }
    flush();
}

void CmdHighlighter::highlightSyntax(HighlightInput &input) {
    parser->run(lexer, &input);
}

CmdHighlighter::~CmdHighlighter() {
    DELETE_MAP(predicates)
    delete empty;
    delete parser;
}
