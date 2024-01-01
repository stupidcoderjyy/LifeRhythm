#include "SyntaxAnalyzer.h"
#include "Compiler.h"
#include "MemUtil.h"
#include "Lexer.h"
#include "CompilerInput.h"
#include "NBT.h"
#include "Error.h"

#define prodSize 16
#define remapSize 132
#define nonTerminalCount 8
#define terminalCount 11
#define statesCount 24

SyntaxAnalyzer::SyntaxAnalyzer(const QString& file) {
    auto* input = CompilerInput::fromFile(file);
    lexer = new Lexer(input);
    goTo = allocateArray(statesCount,nonTerminalCount);
    actions = allocateArray(statesCount,terminalCount);
    productions = new Production*[prodSize];
    suppliers = new PropertySupplier[nonTerminalCount];
    terminalRemap = allocateArray(remapSize);
    initActions();
    initGoTo();
    initOthers();
    initGrammar();
}

NBT* SyntaxAnalyzer::run() {
    auto* input = lexer->input;
    input->mark();
    QVector<int> states{};
    QVector<Property*> properties{};
    states.append(0);
    auto* token = lexer->run();
    auto* eof = TokenFileEnd::get();
    if (token == eof) {
        return nullptr;
    }
    while (true) {
        int s = states.last();
        int order = token ? actions[s][terminalRemap[token->type()]] : 0;
        int type = order >> 16;
        int target = order & 0xFFFF;
        switch(type) {
            case 0: {
                input->recover(false);
                if (token != eof) {
                    delete token;
                }
                for (auto& p : properties) {
                    delete p;
                }
                delete result;
                throw token ?
                    input->errorAtMark("syntax error") :
                    input->errorMarkToForward("unknown symbol");
            }
            case 1: {
                auto* body = new Property*{properties.takeLast()};
                auto* head = suppliers[0]();
                head->onReduced(productions[0], body);
                delete body[0];
                delete head;
                delete body;
                auto* res = result;
                result = nullptr;
                return res;
            }
            case 2: {
                input->mark();
                states.append(target);
                properties.append(new PropertyTerminal(token));
                token = lexer->run();
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
                properties.append(pHead);
                states.append(goTo[states.last()][p->head->id]);
                break;
            }
        }
    }
}

SyntaxAnalyzer::~SyntaxAnalyzer() {
    for (int i = 0 ; i < prodSize ; i ++) {
        delete productions[i];
    }
    for (const auto &item: symbols) {
        delete item;
    }
    delete[] productions;
    delete[] terminalRemap;
    delete[] suppliers;
    delete result;
    delete lexer->input;
    delete lexer;
    freeArray(goTo, statesCount);
    freeArray(actions, statesCount);
}

const int ACCEPT = 0x10000;
const int SHIFT = 0x20000;
const int REDUCE = 0x30000;

void SyntaxAnalyzer::initActions() {
    actions[0][1] = SHIFT | 2;
    actions[1][0] = ACCEPT;
    actions[2][8] = SHIFT | 6;
    actions[2][2] = REDUCE | 9;
    actions[5][8] = SHIFT | 6;
    actions[3][2] = SHIFT | 23;
    actions[4][2] = REDUCE | 11;
    actions[4][8] = REDUCE | 11;
    actions[8][8] = SHIFT | 6;
    actions[8][2] = REDUCE | 9;
    actions[5][2] = REDUCE | 8;
    actions[6][9] = SHIFT | 7;
    actions[7][3] = SHIFT | 10;
    actions[7][4] = SHIFT | 11;
    actions[7][5] = SHIFT | 12;
    actions[7][6] = SHIFT | 13;
    actions[7][1] = SHIFT | 8;
    actions[13][3] = SHIFT | 10;
    actions[13][4] = SHIFT | 11;
    actions[9][2] = REDUCE | 7;
    actions[9][8] = REDUCE | 7;
    actions[13][5] = SHIFT | 12;
    actions[13][6] = SHIFT | 13;
    actions[10][2] = REDUCE | 2;
    actions[10][7] = REDUCE | 2;
    actions[10][8] = REDUCE | 2;
    actions[10][10] = REDUCE | 2;
    actions[17][3] = SHIFT | 10;
    actions[13][1] = SHIFT | 8;
    actions[17][4] = SHIFT | 11;
    actions[11][2] = REDUCE | 3;
    actions[11][7] = REDUCE | 3;
    actions[11][8] = REDUCE | 3;
    actions[11][10] = REDUCE | 3;
    actions[17][5] = SHIFT | 12;
    actions[17][6] = SHIFT | 13;
    actions[12][2] = REDUCE | 4;
    actions[12][7] = REDUCE | 4;
    actions[12][8] = REDUCE | 4;
    actions[12][10] = REDUCE | 4;
    actions[17][1] = SHIFT | 8;
    actions[13][7] = REDUCE | 13;
    actions[14][7] = REDUCE | 15;
    actions[14][10] = REDUCE | 15;
    actions[15][7] = SHIFT | 19;
    actions[16][7] = REDUCE | 12;
    actions[16][10] = SHIFT | 17;
    actions[18][7] = REDUCE | 14;
    actions[18][10] = REDUCE | 14;
    actions[19][2] = REDUCE | 5;
    actions[19][7] = REDUCE | 5;
    actions[19][8] = REDUCE | 5;
    actions[19][10] = REDUCE | 5;
    actions[20][2] = SHIFT | 21;
    actions[21][2] = REDUCE | 6;
    actions[21][7] = REDUCE | 6;
    actions[21][8] = REDUCE | 6;
    actions[21][10] = REDUCE | 6;
    actions[22][2] = REDUCE | 10;
    actions[22][8] = REDUCE | 10;
    actions[23][0] = REDUCE | 1;
}

void SyntaxAnalyzer::initGoTo() {
    goTo[0][1] = 1;
    goTo[2][2] = 3;
    goTo[13][4] = 15;
    goTo[2][6] = 5;
    goTo[13][7] = 16;
    goTo[2][5] = 4;
    goTo[13][3] = 14;
    goTo[5][5] = 22;
    goTo[8][6] = 5;
    goTo[17][3] = 18;
    goTo[8][5] = 4;
    goTo[7][3] = 9;
    goTo[8][2] = 20;
}

void SyntaxAnalyzer::initOthers() {
    terminalRemap[128] = 3;
    terminalRemap[129] = 4;
    terminalRemap[130] = 5;
    terminalRemap[131] = 8;
    terminalRemap[58] = 9;
    terminalRemap[123] = 1;
    terminalRemap[91] = 6;
    terminalRemap[44] = 10;
    terminalRemap[125] = 2;
    terminalRemap[93] = 7;
    suppliers[4] = [](){return new PropertyArr();};
    suppliers[1] = [](){return new PropertyNbt();};
    suppliers[5] = [](){return new PropertyItem();};
    suppliers[2] = [](){return new PropertyCompound();};
    suppliers[0] = [this](){return new PropertyRoot(this);};
    suppliers[6] = [](){return new PropertyItemList();};
    suppliers[7] = [](){return new PropertyElementList();};
    suppliers[3] = [](){return new PropertyElement();};
}

void SyntaxAnalyzer::initGrammar() {
    symbols.push_back(new Symbol(false, 4));
    symbols.push_back(new Symbol(false, 1));
    symbols.push_back(new Symbol(false, 5));
    symbols.push_back(new Symbol(true, 3));
    symbols.push_back(new Symbol(true, 5));
    symbols.push_back(new Symbol(true, 10));
    symbols.push_back(new Symbol(false, 2));
    symbols.push_back(new Symbol(true, 4));
    symbols.push_back(new Symbol(true, -1));
    symbols.push_back(new Symbol(false, 0));
    symbols.push_back(new Symbol(true, 9));
    symbols.push_back(new Symbol(false, 6));
    symbols.push_back(new Symbol(true, 1));
    symbols.push_back(new Symbol(true, 6));
    symbols.push_back(new Symbol(true, 8));
    symbols.push_back(new Symbol(true, 2));
    symbols.push_back(new Symbol(true, 7));
    symbols.push_back(new Symbol(false, 7));
    symbols.push_back(new Symbol(false, 3));
    productions[0] = new Production(0, symbols[9], 1, new Symbol*[]{symbols[1]}); //root → nbt
    productions[1] = new Production(1, symbols[1], 3, new Symbol*[]{symbols[12], symbols[6], symbols[15]}); //nbt → { compound }
    productions[2] = new Production(2, symbols[18], 1, new Symbol*[]{symbols[3]}); //element → string
    productions[3] = new Production(3, symbols[18], 1, new Symbol*[]{symbols[7]}); //element → int
    productions[4] = new Production(4, symbols[18], 1, new Symbol*[]{symbols[4]}); //element → float
    productions[5] = new Production(5, symbols[18], 3, new Symbol*[]{symbols[13], symbols[0], symbols[16]}); //element → [ arr ]
    productions[6] = new Production(6, symbols[18], 3, new Symbol*[]{symbols[12], symbols[6], symbols[15]}); //element → { compound }
    productions[7] = new Production(7, symbols[2], 3, new Symbol*[]{symbols[14], symbols[10], symbols[18]}); //item → id : element
    productions[8] = new Production(8, symbols[6], 1, new Symbol*[]{symbols[11]}); //compound → itemList
    productions[9] = new Production(9, symbols[6], 1, new Symbol*[]{symbols[8]}); //compound → ε
    productions[10] = new Production(10, symbols[11], 2, new Symbol*[]{symbols[11], symbols[2]}); //itemList → itemList item
    productions[11] = new Production(11, symbols[11], 1, new Symbol*[]{symbols[2]}); //itemList → item
    productions[12] = new Production(12, symbols[0], 1, new Symbol*[]{symbols[17]}); //arr → elementList
    productions[13] = new Production(13, symbols[0], 1, new Symbol*[]{symbols[8]}); //arr → ε
    productions[14] = new Production(14, symbols[17], 3, new Symbol*[]{symbols[17], symbols[5], symbols[18]}); //elementList → elementList , element
    productions[15] = new Production(15, symbols[17], 1, new Symbol*[]{symbols[18]}); //elementList → element
}

//arr → elementList
void PropertyArr::reduce0(
        PropertyElementList* p0) {
    array = p0->array;
    p0->array = nullptr;
}

//arr → ε
void PropertyArr::reduce1(
        PropertyTerminal* p0) {
    array = new ArrayData();
}

//nbt → { compound }
void PropertyNbt::reduce0(
        PropertyTerminal* p0,
        PropertyCompound* p1,
        PropertyTerminal* p2) {
    nbt = p1->nbt;
    p1->nbt = nullptr;
}

//item → id : element ;
void PropertyItem::reduce0(
        PropertyTerminal* p0,
        PropertyTerminal* p1,
        PropertyElement* p2) {
    auto* t0 = p0->getToken<TokenId>();
    id = t0->data;
    data = p2->data;
    p2->data = nullptr;
}

//compound → itemList
void PropertyCompound::reduce0(
        PropertyItemList* p0) {
    nbt = p0->nbt;
    p0->nbt = nullptr;
}

//compound → ε
void PropertyCompound::reduce1(
        PropertyTerminal* p0) {
    nbt = new NBT();
}

//root → nbt
void PropertyRoot::reduce0(
        PropertyNbt* p0) {
    parser->result = p0->nbt;
    p0->nbt = nullptr;
}

//itemList → itemList item
void PropertyItemList::reduce0(
        PropertyItemList* p0,
        PropertyItem* p1) {
    nbt = p0->nbt;
    delete nbt->data.value(p1->id, nullptr);
    nbt->data.insert(p1->id, p1->data);
    p0->nbt = nullptr;
    p1->data = nullptr;
}

//itemList → item
void PropertyItemList::reduce1(
        PropertyItem* p0) {
    nbt = new NBT;
    delete nbt->data.value(p0->id, nullptr);
    nbt->data.insert(p0->id, p0->data);
    p0->data = nullptr;
}

//elementList → elementList , element
void PropertyElementList::reduce0(
        PropertyElementList* p0,
        PropertyTerminal* p1,
        PropertyElement* p2) {
    array = p0->array;
    array->data.append(p2->data);
    p0->array = nullptr;
    p2->data = nullptr;
}

//elementList → element
void PropertyElementList::reduce1(
        PropertyElement* p0) {
    array = new ArrayData;
    array->data.append(p0->data);
    p0->data = nullptr;
}

//element → string
void PropertyElement::reduce0(
        PropertyTerminal* p0) {
    auto* t0 = p0->getToken<TokenString>();
    data = (new StringData())->setVal(t0->data);
}

//element → int
void PropertyElement::reduce1(
        PropertyTerminal* p0) {
    auto* t0 = p0->getToken<TokenInt>();
    data = (new IntData())->setVal(t0->data);
}

//element → float
void PropertyElement::reduce2(
        PropertyTerminal* p0) {
    auto* t0 = p0->getToken<TokenFloat>();
    data = (new FloatData())->setVal(t0->data);
}

//element → [ arr ]
void PropertyElement::reduce3(
        PropertyTerminal* p0,
        PropertyArr* p1,
        PropertyTerminal* p2) {
    data = p1->array;
    p1->array = nullptr;
}

//element → { compound }
void PropertyElement::reduce4(
        PropertyTerminal* p0,
        PropertyCompound* p1,
        PropertyTerminal* p2) {
    data = p1->nbt;
    p1->nbt = nullptr;
}

PropertyRoot::PropertyRoot(SyntaxAnalyzer *parser) : parser(parser) {}

void PropertyArr::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 12: reduce0(
                    static_cast<PropertyElementList*>(properties[0]));
            break;
        case 13: reduce1(
                    static_cast<PropertyTerminal*>(properties[0]));
            break;
    }
}

void PropertyNbt::onReduced(Production *p, Property **properties) {
    reduce0(
            static_cast<PropertyTerminal*>(properties[0]),
            static_cast<PropertyCompound*>(properties[1]),
            static_cast<PropertyTerminal*>(properties[2]));
}


void PropertyItem::onReduced(Production *p, Property **properties) {
    reduce0(
            static_cast<PropertyTerminal*>(properties[0]),
            static_cast<PropertyTerminal*>(properties[1]),
            static_cast<PropertyElement*>(properties[2]));
}

void PropertyCompound::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 8: reduce0(
                    static_cast<PropertyItemList*>(properties[0]));
            break;
        case 9: reduce1(
                    static_cast<PropertyTerminal*>(properties[0]));
            break;
    }
}


void PropertyRoot::onReduced(Production *p, Property **properties) {
    reduce0(
            static_cast<PropertyNbt*>(properties[0]));
}

void PropertyItemList::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 10: reduce0(
                    static_cast<PropertyItemList*>(properties[0]),
                    static_cast<PropertyItem*>(properties[1]));
            break;
        case 11: reduce1(
                    static_cast<PropertyItem*>(properties[0]));
            break;
    }
}


void PropertyElementList::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 14: reduce0(
                    static_cast<PropertyElementList*>(properties[0]),
                    static_cast<PropertyTerminal*>(properties[1]),
                    static_cast<PropertyElement*>(properties[2]));
            break;
        case 15: reduce1(
                    static_cast<PropertyElement*>(properties[0]));
            break;
    }
}

void PropertyElement::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 2: reduce0(
                    static_cast<PropertyTerminal*>(properties[0]));
            break;
        case 3: reduce1(
                    static_cast<PropertyTerminal*>(properties[0]));
            break;
        case 4: reduce2(
                    static_cast<PropertyTerminal*>(properties[0]));
            break;
        case 5: reduce3(
                    static_cast<PropertyTerminal*>(properties[0]),
                    static_cast<PropertyArr*>(properties[1]),
                    static_cast<PropertyTerminal*>(properties[2]));
            break;
        case 6: reduce4(
                    static_cast<PropertyTerminal*>(properties[0]),
                    static_cast<PropertyCompound*>(properties[1]),
                    static_cast<PropertyTerminal*>(properties[2]));
            break;
    }
}

PropertyArr::~PropertyArr() {
    delete array;
}

PropertyNbt::~PropertyNbt() {
    delete nbt;
}

PropertyCompound::~PropertyCompound() {
    delete nbt;
}

PropertyItemList::~PropertyItemList() {
    delete nbt;
}

PropertyElementList::~PropertyElementList() {
    delete array;
}

PropertyElement::~PropertyElement() {
    delete data;
}