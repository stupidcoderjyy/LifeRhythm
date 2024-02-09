#include "Parser.h"
#include "Compiler.h"
#include "MemUtil.h"
#include "Lexer.h"
#include "CompilerInput.h"
#include "NBT.h"
#include "Error.h"

#define prodSize 18
#define remapSize 134
#define nonTerminalCount 8
#define terminalCount 13
#define statesCount 26

using namespace snbt;

Parser::Parser(const QString& file) {
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

NBT* Parser::run() {
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

Parser::~Parser() {
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

void Parser::initActions() {
    actions[0][1] = SHIFT | 2;
    actions[1][0] = ACCEPT;
    actions[2][10] = SHIFT | 6;
    actions[2][2] = REDUCE | 11;
    actions[5][10] = SHIFT | 6;
    actions[3][2] = SHIFT | 25;
    actions[4][2] = REDUCE | 13;
    actions[4][10] = REDUCE | 13;
    actions[8][10] = SHIFT | 6;
    actions[8][2] = REDUCE | 11;
    actions[5][2] = REDUCE | 10;
    actions[6][11] = SHIFT | 7;
    actions[7][3] = SHIFT | 10;
    actions[7][4] = SHIFT | 11;
    actions[7][5] = SHIFT | 12;
    actions[7][6] = SHIFT | 13;
    actions[7][7] = SHIFT | 14;
    actions[7][8] = SHIFT | 15;
    actions[7][1] = SHIFT | 8;
    actions[15][3] = SHIFT | 10;
    actions[15][4] = SHIFT | 11;
    actions[9][2] = REDUCE | 9;
    actions[9][10] = REDUCE | 9;
    actions[15][5] = SHIFT | 12;
    actions[15][6] = SHIFT | 13;
    actions[10][2] = REDUCE | 2;
    actions[10][9] = REDUCE | 2;
    actions[10][10] = REDUCE | 2;
    actions[10][12] = REDUCE | 2;
    actions[19][3] = SHIFT | 10;
    actions[15][7] = SHIFT | 14;
    actions[19][4] = SHIFT | 11;
    actions[15][8] = SHIFT | 15;
    actions[11][2] = REDUCE | 3;
    actions[11][9] = REDUCE | 3;
    actions[11][10] = REDUCE | 3;
    actions[11][12] = REDUCE | 3;
    actions[19][5] = SHIFT | 12;
    actions[15][1] = SHIFT | 8;
    actions[19][6] = SHIFT | 13;
    actions[12][2] = REDUCE | 4;
    actions[12][9] = REDUCE | 4;
    actions[12][10] = REDUCE | 4;
    actions[12][12] = REDUCE | 4;
    actions[19][7] = SHIFT | 14;
    actions[19][8] = SHIFT | 15;
    actions[13][2] = REDUCE | 5;
    actions[13][9] = REDUCE | 5;
    actions[13][10] = REDUCE | 5;
    actions[13][12] = REDUCE | 5;
    actions[19][1] = SHIFT | 8;
    actions[14][2] = REDUCE | 6;
    actions[14][9] = REDUCE | 6;
    actions[14][10] = REDUCE | 6;
    actions[14][12] = REDUCE | 6;
    actions[15][9] = REDUCE | 15;
    actions[16][9] = REDUCE | 17;
    actions[16][12] = REDUCE | 17;
    actions[17][9] = SHIFT | 21;
    actions[18][12] = SHIFT | 19;
    actions[18][9] = REDUCE | 14;
    actions[20][9] = REDUCE | 16;
    actions[20][12] = REDUCE | 16;
    actions[21][2] = REDUCE | 7;
    actions[21][9] = REDUCE | 7;
    actions[21][10] = REDUCE | 7;
    actions[21][12] = REDUCE | 7;
    actions[22][2] = SHIFT | 23;
    actions[23][2] = REDUCE | 8;
    actions[23][9] = REDUCE | 8;
    actions[23][10] = REDUCE | 8;
    actions[23][12] = REDUCE | 8;
    actions[24][2] = REDUCE | 12;
    actions[24][10] = REDUCE | 12;
    actions[25][0] = REDUCE | 1;
}

void Parser::initGoTo() {
    goTo[0][1] = 1;
    goTo[2][2] = 3;
    goTo[2][6] = 5;
    goTo[2][5] = 4;
    goTo[15][7] = 18;
    goTo[15][3] = 16;
    goTo[15][4] = 17;
    goTo[5][5] = 24;
    goTo[8][6] = 5;
    goTo[8][5] = 4;
    goTo[7][3] = 9;
    goTo[19][3] = 20;
    goTo[8][2] = 22;
}

void Parser::initOthers() {
    terminalRemap[128] = 3;
    terminalRemap[129] = 4;
    terminalRemap[130] = 5;
    terminalRemap[131] = 6;
    terminalRemap[132] = 7;
    terminalRemap[133] = 10;
    terminalRemap[58] = 11;
    terminalRemap[123] = 1;
    terminalRemap[91] = 8;
    terminalRemap[44] = 12;
    terminalRemap[125] = 2;
    terminalRemap[93] = 9;
    suppliers[4] = [](){return new PropertyArr();};
    suppliers[1] = [](){return new PropertyNbt();};
    suppliers[5] = [](){return new PropertyItem();};
    suppliers[2] = [](){return new PropertyCompound();};
    suppliers[0] = [this](){return new PropertyRoot(this);};
    suppliers[6] = [](){return new PropertyItemList();};
    suppliers[7] = [](){return new PropertyElementList();};
    suppliers[3] = [](){return new PropertyElement();};
}

void Parser::initGrammar() {
    symbols.push_back(new Symbol(false, 4));
    symbols.push_back(new Symbol(false, 1));
    symbols.push_back(new Symbol(false, 5));
    symbols.push_back(new Symbol(true, 6));
    symbols.push_back(new Symbol(true, 3));
    symbols.push_back(new Symbol(true, 7));
    symbols.push_back(new Symbol(true, 5));
    symbols.push_back(new Symbol(true, 12));
    symbols.push_back(new Symbol(false, 2));
    symbols.push_back(new Symbol(true, 4));
    symbols.push_back(new Symbol(true, -1));
    symbols.push_back(new Symbol(false, 0));
    symbols.push_back(new Symbol(true, 11));
    symbols.push_back(new Symbol(false, 6));
    symbols.push_back(new Symbol(true, 1));
    symbols.push_back(new Symbol(true, 8));
    symbols.push_back(new Symbol(true, 10));
    symbols.push_back(new Symbol(true, 2));
    symbols.push_back(new Symbol(true, 9));
    symbols.push_back(new Symbol(false, 7));
    symbols.push_back(new Symbol(false, 3));
    productions[0] = new Production(0, symbols[11], 1, new Symbol*[]{symbols[1]}); //root → nbt
    productions[1] = new Production(1, symbols[1], 3, new Symbol*[]{symbols[14], symbols[8], symbols[17]}); //nbt → { compound }
    productions[2] = new Production(2, symbols[20], 1, new Symbol*[]{symbols[4]}); //element → string
    productions[3] = new Production(3, symbols[20], 1, new Symbol*[]{symbols[9]}); //element → int
    productions[4] = new Production(4, symbols[20], 1, new Symbol*[]{symbols[6]}); //element → float
    productions[5] = new Production(5, symbols[20], 1, new Symbol*[]{symbols[3]}); //element → $true
    productions[6] = new Production(6, symbols[20], 1, new Symbol*[]{symbols[5]}); //element → $false
    productions[7] = new Production(7, symbols[20], 3, new Symbol*[]{symbols[15], symbols[0], symbols[18]}); //element → [ arr ]
    productions[8] = new Production(8, symbols[20], 3, new Symbol*[]{symbols[14], symbols[8], symbols[17]}); //element → { compound }
    productions[9] = new Production(9, symbols[2], 3, new Symbol*[]{symbols[16], symbols[12], symbols[20]}); //item → id : element
    productions[10] = new Production(10, symbols[8], 1, new Symbol*[]{symbols[13]}); //compound → itemList
    productions[11] = new Production(11, symbols[8], 1, new Symbol*[]{symbols[10]}); //compound → ε
    productions[12] = new Production(12, symbols[13], 2, new Symbol*[]{symbols[13], symbols[2]}); //itemList → itemList item
    productions[13] = new Production(13, symbols[13], 1, new Symbol*[]{symbols[2]}); //itemList → item
    productions[14] = new Production(14, symbols[0], 1, new Symbol*[]{symbols[19]}); //arr → elementList
    productions[15] = new Production(15, symbols[0], 1, new Symbol*[]{symbols[10]}); //arr → ε
    productions[16] = new Production(16, symbols[19], 3, new Symbol*[]{symbols[19], symbols[7], symbols[20]}); //elementList → elementList , element
    productions[17] = new Production(17, symbols[19], 1, new Symbol*[]{symbols[20]}); //elementList → element
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
    delete nbt->get().value(p1->id, nullptr);
    nbt->get().insert(p1->id, p1->data);
    p0->nbt = nullptr;
    p1->data = nullptr;
}

//itemList → item
void PropertyItemList::reduce1(
        PropertyItem* p0) {
    nbt = new NBT;
    delete nbt->get().value(p0->id, nullptr);
    nbt->get().insert(p0->id, p0->data);
    p0->data = nullptr;
}

//elementList → elementList , element
void PropertyElementList::reduce0(
        PropertyElementList* p0,
        PropertyTerminal* p1,
        PropertyElement* p2) {
    array = p0->array;
    array->get().append(p2->data);
    p0->array = nullptr;
    p2->data = nullptr;
}

//elementList → element
void PropertyElementList::reduce1(
        PropertyElement* p0) {
    array = new ArrayData;
    array->get().append(p0->data);
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

//element → $true
void PropertyElement::reduce3(
        PropertyTerminal* p0) {
    data = (new BoolData())->setVal(true);
}

//element → $false
void PropertyElement::reduce4(
        PropertyTerminal* p0) {
    data = new BoolData();
}

//element → [ arr ]
void PropertyElement::reduce5(
        PropertyTerminal* p0,
        PropertyArr* p1,
        PropertyTerminal* p2) {
    data = p1->array;
    p1->array = nullptr;
}

//element → { compound }
void PropertyElement::reduce6(
        PropertyTerminal* p0,
        PropertyCompound* p1,
        PropertyTerminal* p2) {
    data = p1->nbt;
    p1->nbt = nullptr;
}

PropertyRoot::PropertyRoot(Parser *parser) : parser(parser) {}

void PropertyArr::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 14: reduce0(
                    static_cast<PropertyElementList*>(properties[0]));
            break;
        case 15: reduce1(
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
        case 10: reduce0(
                    static_cast<PropertyItemList*>(properties[0]));
            break;
        case 11: reduce1(
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
        case 12: reduce0(
                    static_cast<PropertyItemList*>(properties[0]),
                    static_cast<PropertyItem*>(properties[1]));
            break;
        case 13: reduce1(
                    static_cast<PropertyItem*>(properties[0]));
            break;
    }
}


void PropertyElementList::onReduced(Production *p, Property **properties) {
    switch (p->id) {
        case 16: reduce0(
                    static_cast<PropertyElementList*>(properties[0]),
                    static_cast<PropertyTerminal*>(properties[1]),
                    static_cast<PropertyElement*>(properties[2]));
            break;
        case 17: reduce1(
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
                    static_cast<PropertyTerminal*>(properties[0]));
            break;
        case 6: reduce4(
                    static_cast<PropertyTerminal*>(properties[0]));
            break;
        case 7: reduce5(
                    static_cast<PropertyTerminal*>(properties[0]),
                    static_cast<PropertyArr*>(properties[1]),
                    static_cast<PropertyTerminal*>(properties[2]));
            break;
        case 8: reduce6(
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