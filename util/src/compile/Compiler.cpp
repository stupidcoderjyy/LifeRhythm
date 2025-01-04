//
// Created by stupid_coder_jyy on 2023/12/30.
//
#include "Compiler.h"
#include "Helpers.h"
#include "AbstractInput.h"
#include "CompilerInput.h"
#include "Error.h"

USING_LR

TokenFileEnd *TokenFileEnd::get() {
    static auto* instance = new TokenFileEnd();
    return instance;
}

int TokenFileEnd::type() {
    return 0;
}

Token::MatchResult TokenFileEnd::onMatched(const QString &lexeme, AbstractInput *input) {
    return Accept;
}

int TokenSingle::type() {
    return ch;
}

Token::MatchResult TokenSingle::onMatched(const QString &lexeme, AbstractInput *input) {
    ch = lexeme.at(0).cell();
    return Accept;
}

Token *ILexer::nextToken(AbstractInput *input) noexcept {
    return nullptr;
}

DFALexer::DFALexer(int statesCount, int startState):
    statesCount(statesCount),
    startState(startState),
    accepted(new bool[statesCount]),
    goTo(Helpers::allocateArray(statesCount, 128)),
    tokens(new TokenSupplier[statesCount]) {
}

Token *DFALexer::nextToken(AbstractInput *input) noexcept {
    BEGIN:
    input->skip(' ', '\r', '\n');
    input->mark();
    if (!input->available()) {
        return TokenFileEnd::get();
    }
    int state = startState;
    int lastAccepted = -2;
    int extraLoadedBytes = 0;
    while (input->available()){
        int b = input->read();
        if (b < 0) {
            try {
                input->retract();
                input->readUtf();
            } catch (Error&) {
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
        input->approach('\r', ' ', '\t');
        return nullptr;
    }
    input->retract(extraLoadedBytes);
    input->mark();
    Token* token = tokens[lastAccepted]();
    switch (token->onMatched(input->capture(), input)) {
        case Token::Accept:
            return token;
        case Token::Ignore:
            delete token;
            goto BEGIN;
        default:
            delete token;
            return nullptr;
    }
}

DFALexer::~DFALexer() {
    delete[] accepted;
    delete[] tokens;
    Helpers::freeArray(goTo, statesCount);
}

LALRParser::LALRParser(ILexer *lexer, int remap,int nonTerminal, int terminal, int states):
statesCount(states),
actions(Helpers::allocateArray(states, terminal)),
goTo(Helpers::allocateArray(states, nonTerminal)),
terminalRemap(Helpers::allocateArray(remap)),
suppliers(new PropertySupplier[nonTerminal]),
lexer(lexer),
input() {
}

void LALRParser::run(AbstractInput* input) {
    this->input = input;
    input->mark();
    QVector<int> states;
    QVector<Property*> properties;
    states.append(0);
    auto* token = lexer->nextToken(input);
    auto* eof = TokenFileEnd::get();
    if (token == eof) {
        return;
    }
    while (true) {
        int s = states.last();
        int order = token ? actions[s][terminalRemap[token->type()]] : 0;
        int type = order >> 16;
        int target = order & 0xFFFF;
        switch(type) {
            default: {
                input->recover(false);
                if (token != eof) {
                    delete token;
                }
                for (auto& p : properties) {
                    delete p;
                }
                onFailed(token);
                return;
            }
            case 1: {
                auto* body = new Property*{properties.takeLast()};
                auto* head = suppliers[0]();
                head->onReduced(productions[0], body);
                delete body[0];
                delete head;
                delete body;
                onFinished();
                return;
            }
            case 2: {
                input->mark();
                states << target;
                properties << new PropertyTerminal(token);
                token = lexer->nextToken(input);
                onShifted();
                break;
            }
            case 3: {
                auto* p = productions[target];
                auto** body = new Property*[p->bodyLen]; //Property在调用完onReduced后立刻销毁
                for (int i = p->bodyLen - 1 ; i >= 0 ; i --) {
                    auto* symbol = p->body[i];
                    if (symbol->id < 0) {
                        body[i] = nullptr;
                        continue; //ε
                    }
                    states.removeLast();
                    body[i] = properties.takeLast();
                    if (symbol->isTerminal) {
                        input->removeMark();
                    }
                }
                auto* pHead = suppliers[p->head->id]();
                pHead->onReduced(p, body);
                for (int i = 0 ; i < p->bodyLen ; i ++) {
                    delete body[i];
                }
                delete[] body;
                properties << pHead;
                states << goTo[states.last()][p->head->id];
                onReduced();
                break;
            }
        }
    }
}

void LALRParser::onFinished() {
}

void LALRParser::onFailed(Token* at) {
    if (auto ci = dynamic_cast<CompilerInput*>(input)) {
        throw at ?
              ci->errorAtMark("syntax error") :
              ci->errorMarkToForward("unknown symbol");
    }
}

void LALRParser::onReduced() {
}

void LALRParser::onShifted() {
}

LALRParser::~LALRParser() {
    for (auto* p : productions) {
        delete p;
    }
    for (auto* item: symbols) {
        delete item;
    }
    delete[] terminalRemap;
    delete[] suppliers;
    delete lexer;
    Helpers::freeArray(goTo, statesCount);
    Helpers::freeArray(actions, statesCount);
}
