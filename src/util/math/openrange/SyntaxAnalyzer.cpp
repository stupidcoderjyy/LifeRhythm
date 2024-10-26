#include "SyntaxAnalyzer.h"
#include "Compiler.h"
#include "MemUtil.h"
#include "Lexer.h"
#include "CompilerInput.h"
#include "StringByteReader.h"
#include "StringInput.h"

USING_NAMESPACE(lr::openrange)

SyntaxAnalyzer::SyntaxAnalyzer(Lexer* lexer): AbstractSyntaxAnalyzer(lexer, 129, 3, 10, 18) {
    initActions();
    initGoTo();
    initOthers();
    initGrammar();
}

OpenRange SyntaxAnalyzer::parse(const QString &expr) {
    SyntaxAnalyzer analyzer(new Lexer(CompilerInput::fromString(expr)));
    try {
        analyzer.run();
        return analyzer.result;
    } catch (CompileError& err) {
        err.printErr();
    }
    return {};
}

constexpr int ACCEPT = 0x10000;
constexpr int SHIFT = 0x20000;
constexpr int REDUCE = 0x30000;

void SyntaxAnalyzer::initActions() const {
    actions[0][2] = SHIFT | 3;
    actions[15][0] = REDUCE | 3;
    actions[15][1] = REDUCE | 3;
    actions[0][6] = SHIFT | 4;
    actions[0][8] = SHIFT | 5;
    actions[17][0] = REDUCE | 1;
    actions[17][1] = REDUCE | 1;
    actions[1][1] = SHIFT | 16;
    actions[1][0] = ACCEPT;
    actions[2][0] = REDUCE | 2;
    actions[2][1] = REDUCE | 2;
    actions[3][3] = SHIFT | 12;
    actions[4][3] = SHIFT | 8;
    actions[5][3] = SHIFT | 6;
    actions[6][9] = SHIFT | 7;
    actions[16][2] = SHIFT | 3;
    actions[16][6] = SHIFT | 4;
    actions[7][0] = REDUCE | 5;
    actions[7][1] = REDUCE | 5;
    actions[16][8] = SHIFT | 5;
    actions[8][4] = SHIFT | 9;
    actions[9][3] = SHIFT | 10;
    actions[10][7] = SHIFT | 11;
    actions[11][0] = REDUCE | 4;
    actions[11][1] = REDUCE | 4;
    actions[12][4] = SHIFT | 13;
    actions[13][3] = SHIFT | 14;
    actions[14][5] = SHIFT | 15;
}

void SyntaxAnalyzer::initGoTo() const {
    goTo[0][1] = 1;
    goTo[0][2] = 2;
    goTo[16][2] = 17;
}

void SyntaxAnalyzer::initOthers() {
    terminalRemap[128] = 3;
    terminalRemap[85] = 1;
    terminalRemap[40] = 2;
    terminalRemap[41] = 5;
    terminalRemap[91] = 6;
    terminalRemap[123] = 8;
    terminalRemap[44] = 4;
    terminalRemap[93] = 7;
    terminalRemap[125] = 9;
    suppliers[0] = [this]{return new PropertyRoot(this);};
    suppliers[1] = []{return new PropertyElementList();};
    suppliers[2] = []{return new PropertyElement();};
}

void SyntaxAnalyzer::initGrammar() {
    symbols << new Symbol(true, 1);
    symbols << new Symbol(false, 0);
    symbols << new Symbol(true, 3);
    symbols << new Symbol(true, 2);
    symbols << new Symbol(true, 5);
    symbols << new Symbol(true, 6);
    symbols << new Symbol(true, 8);
    symbols << new Symbol(true, 4);
    symbols << new Symbol(false, 1);
    symbols << new Symbol(true, 7);
    symbols << new Symbol(true, 9);
    symbols << new Symbol(false, 2);
    productions << new Production(0, symbols[1], 1, new Symbol*[]{symbols[8]}); //root → elementList
    productions << new Production(1, symbols[8], 3, new Symbol*[]{symbols[8], symbols[0], symbols[11]}); //elementList → elementList U element
    productions << new Production(2, symbols[8], 1, new Symbol*[]{symbols[11]}); //elementList → element
    productions << new Production(3, symbols[11], 5, new Symbol*[]{symbols[3], symbols[2], symbols[7], symbols[2], symbols[4]}); //element → ( num , num )
    productions << new Production(4, symbols[11], 5, new Symbol*[]{symbols[5], symbols[2], symbols[7], symbols[2], symbols[9]}); //element → [ num , num ]
    productions << new Production(5, symbols[11], 3, new Symbol*[]{symbols[6], symbols[2], symbols[10]}); //element → { num }
}

//root → elementList
void PropertyRoot::reduce0(
        PropertyElementList* p0) {
    analyzer->result = std::move(p0->result);
}

//elementList → elementList U element
void PropertyElementList::reduce0(
        PropertyElementList* p0,
        PropertyTerminal* p1,
        PropertyElement* p2) {
    p0->result.unionWith(p2->left, p2->right);
    result = std::move(p0->result);
}

//elementList → element
void PropertyElementList::reduce1(
        PropertyElement* p0) {
    result = OpenRange(p0->left, p0->right);
}

//element → ( num , num )
void PropertyElement::reduce0(
        PropertyTerminal* p0,
        PropertyTerminal* p1,
        PropertyTerminal* p2,
        PropertyTerminal* p3,
        PropertyTerminal* p4) {
    left = p1->getToken<TokenNum>()->val;
    right = p3->getToken<TokenNum>()->val;
}

//element → [ num , num ]
void PropertyElement::reduce1(
        PropertyTerminal* p0,
        PropertyTerminal* p1,
        PropertyTerminal* p2,
        PropertyTerminal* p3,
        PropertyTerminal* p4) {
    left = p1->getToken<TokenNum>()->val - 1;
    right = p3->getToken<TokenNum>()->val + 1;
}

//element → { num }
void PropertyElement::reduce2(
        PropertyTerminal* p0,
        PropertyTerminal* p1,
        PropertyTerminal* p2) {
    int val = p1->getToken<TokenNum>()->val;
    left = val - 1;
    right = val + 1;
}

PropertyRoot::PropertyRoot(SyntaxAnalyzer *analyzer): analyzer(analyzer) {
}

void PropertyRoot::onReduced(Production *p, Property **properties) {
    reduce0(
            static_cast<PropertyElementList*>(properties[0]));
}

void PropertyElementList::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 1: reduce0(
                static_cast<PropertyElementList*>(properties[0]),
                static_cast<PropertyTerminal*>(properties[1]),
                static_cast<PropertyElement*>(properties[2]));
        break;
        case 2: reduce1(
                static_cast<PropertyElement*>(properties[0]));
        break;
    }
}

void PropertyElement::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 3: reduce0(
                static_cast<PropertyTerminal*>(properties[0]),
                static_cast<PropertyTerminal*>(properties[1]),
                static_cast<PropertyTerminal*>(properties[2]),
                static_cast<PropertyTerminal*>(properties[3]),
                static_cast<PropertyTerminal*>(properties[4]));
        break;
        case 4: reduce1(
                static_cast<PropertyTerminal*>(properties[0]),
                static_cast<PropertyTerminal*>(properties[1]),
                static_cast<PropertyTerminal*>(properties[2]),
                static_cast<PropertyTerminal*>(properties[3]),
                static_cast<PropertyTerminal*>(properties[4]));
        break;
        case 5: reduce2(
                static_cast<PropertyTerminal*>(properties[0]),
                static_cast<PropertyTerminal*>(properties[1]),
                static_cast<PropertyTerminal*>(properties[2]));
        break;
    }
}

