#include "CmdHighlightParser.h"
#include "Compiler.h"
#include "CmdHighlightLexer.h"
#include "CmdManager.h"
#include "Styles.h"
#include "CmdHighlighter.h"

USING_NP(lr::cmd::highlight)

CmdHighlightParser::CmdHighlightParser(CmdHighlighter* highlighter, CmdManager *manager):
        LALRParser(130, 6, 4, 11),
        highlighter(highlighter),
        manager(manager),
        cmd() {
    initActions();
    initGoTo();
    initOthers();
    initGrammar();
}

constexpr int ACCEPT = 0x10000;
constexpr int SHIFT = 0x20000;
constexpr int REDUCE = 0x30000;

void CmdHighlightParser::initActions() const {
    actions[0][2] = SHIFT | 3;
    actions[1][0] = ACCEPT;
    actions[2][0] = REDUCE | 6;
    actions[2][3] = SHIFT | 8;
    actions[3][0] = REDUCE | 4;
    actions[3][1] = REDUCE | 4;
    actions[3][3] = REDUCE | 4;
    actions[4][1] = SHIFT | 5;
    actions[4][0] = REDUCE | 2;
    actions[4][3] = REDUCE | 2;
    actions[5][2] = SHIFT | 6;
    actions[6][0] = REDUCE | 3;
    actions[6][1] = REDUCE | 3;
    actions[6][3] = REDUCE | 3;
    actions[7][0] = REDUCE | 1;
    actions[8][0] = REDUCE | 8;
    actions[8][3] = REDUCE | 8;
    actions[9][0] = REDUCE | 5;
    actions[9][3] = SHIFT | 10;
    actions[10][0] = REDUCE | 7;
    actions[10][3] = REDUCE | 7;
}

void CmdHighlightParser::initGoTo() const {
    goTo[0][1] = 1;
    goTo[0][2] = 2;
    goTo[0][4] = 4;
    goTo[2][3] = 7;
    goTo[2][5] = 9;
}

void CmdHighlightParser::initOthers() {
    terminalRemap[128] = 2;
    terminalRemap[129] = 3;
    terminalRemap[46] = 1;
    suppliers[3] = [] { return new PropertyArgs(); };
    suppliers[5] = [this] { return new PropertyArgList(this); };
    suppliers[0] = [] { return new PropertyRoot(); };
    suppliers[1] = [this] { return new PropertyExpr(this); };
    suppliers[2] = [this] { return new PropertyCmd(this); };
    suppliers[4] = [this] { return new PropertyNamespaces(this); };
}

void CmdHighlightParser::initGrammar() {
    symbols << new Symbol(false, 3);
    symbols << new Symbol(false, 5);
    symbols << new Symbol(true, -1);
    symbols << new Symbol(false, 0);
    symbols << new Symbol(true, 3);
    symbols << new Symbol(false, 1);
    symbols << new Symbol(false, 2);
    symbols << new Symbol(true, 2);
    symbols << new Symbol(true, 1);
    symbols << new Symbol(false, 4);
    productions << new Production(0, symbols[3], 1, new Symbol *[]{symbols[5]}); //root → expr
    productions << new Production(1, symbols[5], 2, new Symbol *[]{symbols[6], symbols[0]}); //expr → cmd args
    productions << new Production(2, symbols[6], 1, new Symbol *[]{symbols[9]}); //cmd → namespaces
    productions << new Production(3, symbols[9], 3, new Symbol *[]{symbols[9], symbols[8], symbols[7]}); //namespaces → namespaces . id
    productions << new Production(4, symbols[9], 1, new Symbol *[]{symbols[7]}); //namespaces → id
    productions << new Production(5, symbols[0], 1, new Symbol *[]{symbols[1]}); //args → argList
    productions << new Production(6, symbols[0], 1, new Symbol *[]{symbols[2]}); //args → ε
    productions << new Production(7, symbols[1], 2, new Symbol *[]{symbols[1], symbols[4]}); //argList → argList arg
    productions << new Production(8, symbols[1], 1, new Symbol *[]{symbols[4]}); //argList → arg
}

//args → argList
void PropertyArgs::reduce0(
    PropertyArgList *p0) {
}

//args → ε
void PropertyArgs::reduce1(
    PropertyTerminal *p0) {
}

//argList → argList arg
void PropertyArgList::reduce0(
    PropertyArgList *p0,
    PropertyTerminal *p1) {
    if (parser->cmd) {
        pos = p0->pos + 1;
        auto predicate = highlighter->getPredicate(parser->cmd->getId());
        auto arg = p1->getToken<TokenArg>()->getArg();
        highlighter->replacePeek(predicate->checkArgument(pos, arg));
    }
}

//argList → arg
void PropertyArgList::reduce1(
    PropertyTerminal *p0) {
    if (parser->cmd) {
        auto predicate = highlighter->getPredicate(parser->cmd->getId());
        auto arg = p0->getToken<TokenArg>()->getArg();
        highlighter->replacePeek(predicate->checkArgument(pos, arg));
    }
}

//root → expr
void PropertyRoot::reduce0(
    PropertyExpr *p0) {
}

//expr → cmd args
void PropertyExpr::reduce0(
    PropertyCmd *p0,
    PropertyArgs *p1) {
    highlighter->flush();
}

//cmd → namespaces
void PropertyCmd::reduce0(
    PropertyNamespaces *p0) {
    parser->cmd == p0->parent->getCommand(p0->id);
    if (parser->cmd) {
        highlighter->replacePeek("cmd");
    }
    highlighter->flush();
}

//namespaces → namespaces . id
void PropertyNamespaces::reduce0(
    PropertyNamespaces *p0,
    PropertyTerminal *p1,
    PropertyTerminal *p2) {
    auto tokenId = p2->getToken<TokenId>();
    if (!p0->node) {
        highlighter->replacePeek(Styles::FORMAT_ERROR);
    }
    parent = p0->node;
    id = tokenId->getId();
    auto child = parent->getChild(id);
    if (child) {
        node = child;
    } else {
        highlighter->replacePeek(Styles::FORMAT_ERROR);
    }
}

//namespaces → id
void PropertyNamespaces::reduce1(
    PropertyTerminal *p0) {
    auto tokenId = p0->getToken<TokenId>();
    parent = manager->getRoot();
    id = tokenId->getId();
    auto child = parent->getChild(tokenId->getId());
    if (child) {
        node = child;
    } else {
        highlighter->replacePeek(Styles::FORMAT_ERROR);
    }
}

void PropertyArgs::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 5: reduce0(
                static_cast<PropertyArgList *>(properties[0]));
            break;
        case 6: reduce1(
                static_cast<PropertyTerminal *>(properties[0]));
            break;
    }
}

PropertyArgList::PropertyArgList(CmdHighlightParser *parser): parser(parser), highlighter(parser->getHighlighter()), pos() {
}

void PropertyArgList::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 7: reduce0(
                static_cast<PropertyArgList *>(properties[0]),
                static_cast<PropertyTerminal *>(properties[1]));
            break;
        case 8: reduce1(
                static_cast<PropertyTerminal *>(properties[0]));
            break;
    }
}

void PropertyRoot::onReduced(Production *p, Property **properties) {
    reduce0(
        static_cast<PropertyExpr *>(properties[0]));
}

PropertyExpr::PropertyExpr(CmdHighlightParser *parser): highlighter(parser->getHighlighter()) {
}

void PropertyExpr::onReduced(Production *p, Property **properties) {
    reduce0(
        static_cast<PropertyCmd *>(properties[0]),
        static_cast<PropertyArgs *>(properties[1]));
}

PropertyCmd::PropertyCmd(CmdHighlightParser *parser): parser(parser), highlighter(parser->getHighlighter()) {
}

void PropertyCmd::onReduced(Production *p, Property **properties) {
    reduce0(
        static_cast<PropertyNamespaces *>(properties[0]));
}

PropertyNamespaces::PropertyNamespaces(CmdHighlightParser *parser):
        highlighter(parser->getHighlighter()), manager(parser->getCommandManager()), node(), parent() {
}

void PropertyNamespaces::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 3: reduce0(
                static_cast<PropertyNamespaces *>(properties[0]),
                static_cast<PropertyTerminal *>(properties[1]),
                static_cast<PropertyTerminal *>(properties[2]));
            break;
        case 4: reduce1(
                static_cast<PropertyTerminal *>(properties[0]));
            break;
    }
}
