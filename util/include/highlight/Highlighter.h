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
private:
    QVector<StyleUnit> styles;
    QMap<QString, QTextCharFormat>* styleGroup;
    bool shouldFlush;
protected:
    HighlightLexer* lexer;
public:
    explicit Highlighter(const Identifier& styleGroup, QTextDocument* doc, HighlightLexer *lexer);
    void push(const QTextCharFormat& unit, int begin, int end);
    void push(const QTextCharFormat& unit);
    void push(const QString& style, int begin, int end);
    void push(const QString& style);
    StyleUnit pop();
    StyleUnit get(int i) const;
    void flush();
    ~Highlighter() override;
protected:
    void highlightBlock(const QString &text) override;
    virtual void prepareStyles(HighlightInput &input);
};

END_NP

#endif //HIGHLIGHTER_H
