//
// Created by JYY on 25-1-1.
//

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include "CompilerInput.h"
#include "Identifier.h"
#include "Namespaces.h"

BEGIN_LR

class HighlightInput;
class HighlightLexer;

struct StyleUnit {
    QTextCharFormat style;
    int begin;
    int end;
};

class Highlighter : public QSyntaxHighlighter {
public:
    enum Mode {
        Lex,
        Syntax
    };
private:
    QVector<StyleUnit> styles;
    QMap<QString, QTextCharFormat>* styleGroup;
protected:
    HighlightLexer* lexer;
    Mode mode;
public:
    explicit Highlighter(const Identifier& styleGroup, QTextDocument* doc, HighlightLexer *lexer);
    void push(const QTextCharFormat& fmt, int begin, int end);
    void push(const QTextCharFormat& fmt);
    void push(const QString& style, int begin, int end);
    void push(const QString& style);
    void replacePeek(const QTextCharFormat &fmt);
    void replacePeek(const QString &style);
    StyleUnit pop();
    StyleUnit get(int i) const;
    void flush();
    void setMode(Mode mode) { this->mode = mode; }
    ~Highlighter() override;
protected:
    void highlightBlock(const QString& text) override;
    virtual void highlightLex(HighlightInput& input) {}
    virtual void highlightSyntax(HighlightInput& input) {}
};

END_NP

#endif //HIGHLIGHTER_H
