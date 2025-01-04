//
// Created by JYY on 25-1-3.
//

#ifndef HIGHTLIGHTLEXER_H
#define HIGHTLIGHTLEXER_H
#include "Compiler.h"
#include "Namespaces.h"

BEGIN_LR

class Highlighter;
class HighlightInput;

class HighlightLexer : public DFALexer {
    friend class SyntaxAnalyzer;
protected:
    Highlighter *highlighter;
    HighlightInput* input;
    int beginPos;
    int endPos;
public:
    explicit HighlightLexer(Highlighter *highlighter, int statesCount, int startState);
    Token *nextToken(AbstractInput* input) noexcept override;
    inline int begin() const;
    inline int end() const;
};

inline int HighlightLexer::begin() const {
    return beginPos;
}

inline int HighlightLexer::end() const {
    return endPos;
}

END_NP

#endif //HIGHTLIGHTLEXER_H
