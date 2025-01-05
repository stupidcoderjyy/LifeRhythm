//
// Created by JYY on 25-1-3.
//

#ifndef CMDHIGHLIGHTER_H
#define CMDHIGHLIGHTER_H

#include "CmdHighlightLexer.h"
#include "CmdHighlightParser.h"
#include "Highlighter.h"
#include "Namespaces.h"
#include "Styles.h"

BEGIN_NP(lr::cmd::highlight)

class ArgumentPredicate {
public:
    virtual ~ArgumentPredicate() = default;
    virtual QTextCharFormat checkArgument(int i, const QString& arg) { return Styles::FORMAT_ERROR; }
    virtual bool shouldDelete() { return true; }
};

class CmdHighlighter : public Highlighter {
private:
    CmdHighlightParser* parser;
    ArgumentPredicate* empty;
    QMap<Identifier, ArgumentPredicate*> predicates;
public:
    explicit CmdHighlighter(const Identifier &styleGroup, QTextDocument *doc, CmdManager* manager);
    void registerPredicate(const Identifier& loc, ArgumentPredicate* predicate);
    ArgumentPredicate* getPredicate(const Identifier& loc) const;
    void highlightLex(HighlightInput &input) override;
    void highlightSyntax(HighlightInput &input) override;
    ~CmdHighlighter() override;
};

END_NP

#endif //CMDHIGHLIGHTER_H
