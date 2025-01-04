//
// Created by JYY on 25-1-3.
//

#include "HighlightLexer.h"
#include "Error.h"
#include "HighlightInput.h"

USING_LR

HighlightLexer::HighlightLexer(Highlighter *h, int statesCount, int startState): DFALexer(statesCount, startState), highlighter(h), input(), beginPos(), endPos() {
}

Token *HighlightLexer::nextToken(AbstractInput *i) noexcept {
    input = static_cast<HighlightInput*>(i);
BEGIN:
    input->skip(' ', '\r', '\n');
    if (!input->available()) {
        return TokenFileEnd::get();
    }
    input->mark();
    int state = startState;
    int lastAccepted = -2;
    int extraLoadedBytes = 0;
    beginPos = input->pos();
    while (input->available()) {
        int b = input->read();
        if (b < 0) {
            try {
                input->retract();
                input->readUtf();
            } catch (Error &) {
                endPos = input->pos();
                return nullptr; //不接受非UTF字符
            }
            b = 1; //UTF字符视为一个用不到的ASCII控制字符
        }
        state = goTo[state][b];
        if (state == 0) {
            extraLoadedBytes++;
            break;
        }
        if (accepted[state]) {
            lastAccepted = state;
            extraLoadedBytes = 0;
        } else {
            extraLoadedBytes++;
        }
    }
    if (lastAccepted < 0 || !tokens[lastAccepted]) {
        input->approachUtf('\r', ' ', '\t');
        endPos = input->pos();
        return nullptr;
    }
    input->retractUtf(extraLoadedBytes);
    input->mark();
    Token *token = tokens[lastAccepted]();
    switch (token->onMatched(input->capture(), input)) {
        case Token::Accept:
            endPos = input->pos();
            return token;
        case Token::Ignore:
            delete token;
            goto BEGIN;
        default:
            delete token;
            endPos = input->pos();
            return nullptr;
    }
}
