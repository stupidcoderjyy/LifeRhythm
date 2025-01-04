//
// Created by JYY on 25-1-3.
//

#ifndef CMDHIGHLIGHTER_H
#define CMDHIGHLIGHTER_H

#include "CmdLexer.h"
#include "Highlighter.h"
#include "Namespaces.h"

BEGIN_NP(lr::cmd)

class CmdHighlighter : public Highlighter {
public:
    explicit CmdHighlighter(const Identifier &styleGroup, QTextDocument *doc);
    void prepareStyles(HighlightInput &input) override;
};

END_NP

#endif //CMDHIGHLIGHTER_H
