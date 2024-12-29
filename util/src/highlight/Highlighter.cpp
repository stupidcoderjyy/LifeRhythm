//
// Created by stupid_coder_jyy on 2024/2/13.
//

#include "Highlighter.h"

#include "Helpers.h"
#include "StringByteReader.h"
#include "StyleGroupStorage.h"
#include "Styles.h"

USING_LR
USING_NP(lr::highlight)

Input::Input(const QString& blockText):StringInput(blockText){
}

int Input::read() {
    pos++;
    return StringInput::read();
}

QString Input::readUtf() {
    int prev = pos;
    auto res = StringInput::readUtf();
    pos = prev + 1;
    return res;
}

void Input::retractUtf(int count) {
    int ch;
    int bytes = 0;
    for (int i = 0 ; i < count ; i ++) {
        do {
            ch = retract() & 0xFF;
            bytes++;
        } while (ch >> 6 == 2);
    }
    pos += (bytes - count);
}

int Input::retract() {
    pos--;
    return StringInput::retract();
}

int Input::retract(int count) {
    return StringInput::retract(count);
}

int Token::type() {
    return -1;
}

Token::Op Token::onMatched(const QString &lexeme, Input *input, Context* ctx) {
    return Everything;
}

int TokenSingle::type() {
    return ch;
}

Token::Op TokenSingle::onMatched(const QString &lexeme, Input *input, Context* ctx) {
    ch = lexeme.at(0).cell();
    return Everything;
}

TokenError::TokenError(ErrorType type): errType(type) {
    this->fmt = Styles::FORMAT_ERROR;
}

int TokenError::type() {
    return -1;
}

Lexer::Lexer(const Identifier& styleGroupLoc, int statesCount, int startState):
        statesCount(statesCount),
        startState(startState),
        accepted(new bool[statesCount]),
        goTo(Helpers::allocateArray(statesCount, 128)),
        tokens(new TokenSupplier[statesCount]){
    styleGroup = StyleGroupStorage::get(styleGroupLoc);
    if (!styleGroup) {
        throw Error("style group not found " + styleGroupLoc.toString());
    }
}

void Lexer::reset(Input *in) {
    input = in;
}

Token *Lexer::run(Context* ctx) noexcept {
    input->skip(' ', '\r', '\n');
    if (!input->available()) {
        return nullptr;
    }
    input->mark();
    int state = startState;
    int lastAccepted = -2;
    int extraLoaded = 0;
    int begin = input->pos;
    while (input->available()){
        int b = input->read();
        if (b < 0) {
            try {
                input->retract();
                input->readUtf();
            } catch (Error&) {
                return new TokenError(TokenError::InvalidEncoding);
            }
            b = 1; //UTF字符视为一个用不到的ASCII控制字符
        }
        state = goTo[state][b];
        if (state == 0) {
            extraLoaded++;
            break;
        }
        if (accepted[state]) {
            lastAccepted = state;
            extraLoaded = 0;
        } else {
            extraLoaded++;
        }
    }
    if (lastAccepted < 0 || !tokens[lastAccepted]) {
        auto* tokenErr = new TokenError(TokenError::InvalidSymbol);
        while (input->available()) {
            auto ch = input->readUtf();
            if (ch == ' ' || ch == '\r') {
                break;
            }
        }
        tokenErr->begin = begin;
        tokenErr->end = input->pos;
        input->removeMark();
        return tokenErr;
    }
    input->retractUtf(extraLoaded);
    Token* token = tokens[lastAccepted]();
    token->begin = begin;
    input->mark();
    switch (token->onMatched(input->capture(), input, ctx)) {
        case Token::Everything: {
            token->end = input->pos;
            QString styleName = idToStyleName.value(token->type());
            token->fmt = styleGroup->value(styleName, Styles::FORMAT_DEFAULT);
            break;
        }
        case Token::NoEnd: {
            QString styleName = idToStyleName.value(token->type());
            token->fmt = styleGroup->value(styleName, Styles::FORMAT_DEFAULT);
            break;
        }
        case Token::NoFormat: {
            token->end = input->pos;
        }
        case Token::NoEndAndFormat:
            break;
    }
    return token;
}

Lexer::~Lexer() {
    delete[] accepted;
    delete[] tokens;
    Helpers::freeArray(goTo, statesCount);
}
