//
// Created by JYY on 25-1-1.
//

#include "Highlighter.h"
#include "HighlightLexer.h"
#include "HighlightInput.h"
#include "StyleGroupStorage.h"
#include "Styles.h"

USING_NP(lr)

Highlighter::Highlighter(const Identifier &styleGroup, QTextDocument *doc, HighlightLexer *lexer): QSyntaxHighlighter(doc),
styleGroup(StyleGroupStorage::get(styleGroup)), shouldFlush(), lexer(lexer) {
}

void Highlighter::push(const QTextCharFormat &f, int begin, int end) {
    styles.append({f, begin, end});
}

void Highlighter::push(const QTextCharFormat& f) {
    styles.append({f, lexer->begin(), lexer->end()});
}

void Highlighter::push(const QString &style, int begin, int end) {
    if (styleGroup && styleGroup->contains(style)) {
        styles.append({styleGroup->value(style), begin, end});
    } else {
        styles.append({Styles::FORMAT_DEFAULT, begin, end});
    }
}

void Highlighter::push(const QString &style) {
    push(style, lexer->begin(), lexer->end());
}

StyleUnit Highlighter::get(int i) const {
    return styles.at(i);
}

void Highlighter::flush() {
    shouldFlush = true;
}

Highlighter::~Highlighter() {
    delete lexer;
}

StyleUnit Highlighter::pop() {
    return styles.takeLast();
}

void Highlighter::highlightBlock(const QString &text) {
    auto ci = HighlightInput(text);
    while (ci.available()) {
        prepareStyles(ci);
        if (shouldFlush) {
            for (const auto& [style, begin, end] : styles) {
                setFormat(begin, end - begin, style);
            }
            styles.clear();
            shouldFlush = false;
        }
    }
}

void Highlighter::prepareStyles(HighlightInput &input) {
}
