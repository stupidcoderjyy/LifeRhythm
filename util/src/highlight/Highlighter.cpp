//
// Created by JYY on 25-1-1.
//

#include "Highlighter.h"
#include "HighlightLexer.h"
#include "HighlightInput.h"
#include "StyleGroupStorage.h"
#include "Styles.h"

USING_NP(lr)

Highlighter::Highlighter(const Identifier &styleGroup, QTextDocument *doc, HighlightLexer *lexer):
        QSyntaxHighlighter(doc),
        styleGroup(StyleGroupStorage::get(styleGroup)),
        lexer(lexer), mode(Syntax) {
}

void Highlighter::push(const QTextCharFormat &f, int begin, int end) {
    styles.append({f, begin, end});
}

void Highlighter::push(const QTextCharFormat& f) {
    styles.append({f, lexer->getBegin(), lexer->getEnd()});
}

void Highlighter::push(const QString &style, int begin, int end) {
    if (styleGroup && styleGroup->contains(style)) {
        styles.append({styleGroup->value(style), begin, end});
    } else {
        styles.append({Styles::FORMAT_DEFAULT, begin, end});
    }
}

void Highlighter::push(const QString &style) {
    push(style, lexer->getBegin(), lexer->getEnd());
}

void Highlighter::replacePeek(const QTextCharFormat &fmt) {
    if (styles.length() > 0) {
        styles.back().style = fmt;
    }
}

void Highlighter::replacePeek(const QString &style) {
    if (styles.length() > 0 && styleGroup && styleGroup->contains(style)) {
        styles.back().style = styleGroup->value(style);
    }
}

StyleUnit Highlighter::get(int i) const {
    return styles.at(i);
}

void Highlighter::flush() {
    for (const auto &[style, begin, end]: styles) {
        setFormat(begin, end - begin, style);
    }
    styles.clear();
}

Highlighter::~Highlighter() {
    delete lexer;
}

StyleUnit Highlighter::pop() {
    return styles.takeLast();
}

void Highlighter::highlightBlock(const QString &text) {
    auto ci = HighlightInput(text);
    if (mode == Lex) {
        highlightLex(ci);
    } else {
        highlightSyntax(ci);
    }
}
