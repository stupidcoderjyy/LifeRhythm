
#ifndef CMD_HIGHLIGHT_PARSER_H
#define CMD_HIGHLIGHT_PARSER_H

#include "Compiler.h"

BEGIN_NP(lr)
BEGIN_NP(cmd)
class Command;
class CmdManager;
class CommandContainer;
END_NP
END_NP

BEGIN_NP(lr::cmd::highlight)

class CmdHighlightLexer;
class CmdHighlighter;

class CmdHighlightParser final : public LALRParser {
    friend class PropertyCmd;
    friend class PropertyArgList;
private:
    CmdHighlighter* highlighter;
    CmdManager* manager;
    Command* cmd;
public:
    explicit CmdHighlightParser(CmdHighlighter* highlighter, CmdManager *manager);
    CmdHighlighter* getHighlighter() const { return this->highlighter; }
    CmdManager* getCommandManager() const { return this->manager; }
private:
    void initActions() const;
    void initGoTo() const;
    void initOthers();
    void initGrammar();
};

class PropertyArgs;
class PropertyArgList;
class PropertyRoot;
class PropertyExpr;
class PropertyCmd;
class PropertyNamespaces;

class PropertyArgs final : public Property {
public:
    void onReduced(Production *p, Property **properties) override;
private:
    void reduce0(PropertyArgList* p0); //args → argList
    void reduce1(PropertyTerminal* p0); //args → ε
};

class PropertyArgList final : public Property {
private:
    int pos;
    CmdHighlightParser *parser;
    CmdHighlighter* highlighter;
public:
    explicit PropertyArgList(CmdHighlightParser* parser);
    void onReduced(Production *p, Property **properties) override;
private:
    void reduce0(PropertyArgList* p0,PropertyTerminal* p1); //argList → argList arg
    void reduce1(PropertyTerminal* p0); //argList → arg
};

class PropertyRoot final : public Property {
public:
    void onReduced(Production *p, Property **properties) override;
private:
    void reduce0(PropertyExpr* p0); //root → expr
};

class PropertyExpr final : public Property {
private:
    CmdHighlighter* highlighter;
public:
    explicit PropertyExpr(CmdHighlightParser* parser);
    void onReduced(Production *p, Property **properties) override;
private:
    void reduce0(PropertyCmd* p0,PropertyArgs* p1); //expr → cmd args
};

class PropertyCmd final : public Property {
private:
    CmdHighlightParser *parser;
    CmdHighlighter* highlighter;
public:
    explicit PropertyCmd(CmdHighlightParser* parser);
    void onReduced(Production *p, Property **properties) override;
private:
    void reduce0(PropertyNamespaces* p0); //cmd → namespaces
};

class PropertyNamespaces final : public Property {
    friend class PropertyCmd;
private:
    CmdHighlighter* highlighter;
    CmdManager* manager;
    CommandContainer* node;
    CommandContainer* parent;
    QString id;
public:
    explicit PropertyNamespaces(CmdHighlightParser* parser);
    void onReduced(Production *p, Property **properties) override;
private:
    void reduce0(PropertyNamespaces* p0,PropertyTerminal* p1,PropertyTerminal* p2); //namespaces → namespaces . id
    void reduce1(PropertyTerminal* p0); //namespaces → id
};

END_NP

#endif
