
#ifndef LIFERHYTHM_SYNTAXANALYZER_H
#define LIFERHYTHM_SYNTAXANALYZER_H

#include "Compiler.h"
#include "Namespaces.h"
#include "OpenRange.h"

BEGIN_NAMESPACE(lr::openrange)

class Lexer;

class SyntaxAnalyzer final : public AbstractSyntaxAnalyzer {
    friend class PropertyRoot;
private:
    OpenRange result;
public:
    explicit SyntaxAnalyzer(Lexer* lexer);
    static OpenRange parse(const QString& expr);
private:
    void initActions() const;
    void initGoTo() const;
    void initOthers();
    void initGrammar();
};

class PropertyRoot;
class PropertyElementList;
class PropertyElement;

class PropertyRoot final : public Property {
private:
    SyntaxAnalyzer* analyzer;
public:
    explicit PropertyRoot(SyntaxAnalyzer* analyzer);
    void onReduced(Production *p, Property **properties) override;
private:
    void reduce0(PropertyElementList* p0); //root → elementList
};

class PropertyElementList final : public Property {
public:
    OpenRange result;
public:
    void onReduced(Production *p, Property **properties) override;
private:
    void reduce0(PropertyElementList* p0,PropertyTerminal* p1,PropertyElement* p2); //elementList → elementList U element
    void reduce1(PropertyElement* p0); //elementList → element
};

class PropertyElement final : public Property {
public:
    int left, right;
public:
    void onReduced(Production *p, Property **properties) override;
private:
    void reduce0(PropertyTerminal* p0,PropertyTerminal* p1,PropertyTerminal* p2,PropertyTerminal* p3,PropertyTerminal* p4); //element → ( num , num )
    void reduce1(PropertyTerminal* p0,PropertyTerminal* p1,PropertyTerminal* p2,PropertyTerminal* p3,PropertyTerminal* p4); //element → [ num , num ]
    void reduce2(PropertyTerminal* p0,PropertyTerminal* p1,PropertyTerminal* p2); //element → { num }
};

END_NAMESPACE

#endif
