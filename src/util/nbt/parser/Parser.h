
#ifndef LIFERHYTHM_PARSER_H
#define LIFERHYTHM_PARSER_H

#include "functional"
#include "Compiler.h"
#include "Data.h"
#include <memory>

class NBT;

namespace snbt {
    class Lexer;
    class Parser{
        friend class PropertyRoot;
    public:
        typedef std::function<Property*()> PropertySupplier;
    private:
        int** actions{};
        int** goTo{};
        int* terminalRemap{};
        Production** productions{};
        PropertySupplier* suppliers{};
        Lexer* lexer{};
        std::vector<Symbol*> symbols{};
        NBT* result{};

    public:
        explicit Parser(const QString& file);
        NBT* run();
        virtual ~Parser();
    private:
        void initActions();
        void initGoTo();
        void initOthers();
        void initGrammar();
    };

    class PropertyArr;
    class PropertyNbt;
    class PropertyItem;
    class PropertyCompound;
    class PropertyRoot;
    class PropertyItemList;
    class PropertyElementList;
    class PropertyElement;

    class PropertyArr : public Property{
        friend class PropertyElement;
    private:
        ArrayData* array{};
    public:
        ~PropertyArr() override;
        void onReduced(Production *p, Property **properties) override;
    private:
        void reduce0(PropertyElementList* p0); //arr → elementList
        void reduce1(PropertyTerminal* p0); //arr → ε
    };

    class PropertyNbt : public Property{
        friend class PropertyRoot;
    private:
        NBT* nbt{};
    public:
        ~PropertyNbt() override;
        void onReduced(Production *p, Property **properties) override;
    private:
        void reduce0(PropertyTerminal* p0,PropertyCompound* p1,PropertyTerminal* p2); //nbt → { compound }
    };

    class PropertyItem : public Property{
    public:
        QString id;
        Data* data{};
    public:
        void onReduced(Production *p, Property **properties) override;
    private:
        void reduce0(PropertyTerminal* p0,PropertyTerminal* p1,PropertyElement* p2); //item → id : element
    };

    class PropertyCompound : public Property{
        friend class PropertyNbt;
        friend class PropertyElement;
    private:
        NBT* nbt{};
    public:
        ~PropertyCompound() override;
        void onReduced(Production *p, Property **properties) override;
    private:
        void reduce0(PropertyItemList* p0); //compound → itemList
        void reduce1(PropertyTerminal* p0); //compound → ε
    };

    class PropertyRoot : public Property{
    private:
        Parser* parser;
    public:
        explicit PropertyRoot(Parser *parser);
        void onReduced(Production *p, Property **properties) override;
    private:
        void reduce0(PropertyNbt* p0); //root → nbt
    };

    class PropertyItemList : public Property{
        friend class PropertyCompound;
    private:
        NBT* nbt{};
    public:
        ~PropertyItemList() override;
        void onReduced(Production *p, Property **properties) override;
    private:
        void reduce0(PropertyItemList* p0,PropertyItem* p1); //itemList → itemList item
        void reduce1(PropertyItem* p0); //itemList → item
    };

    class PropertyElementList : public Property{
        friend class PropertyArr;
    private:
        ArrayData* array{};
    public:
        ~PropertyElementList() override;
        void onReduced(Production *p, Property **properties) override;
    private:
        void reduce0(PropertyElementList* p0,PropertyTerminal* p1,PropertyElement* p2); //elementList → elementList , element
        void reduce1(PropertyElement* p0); //elementList → element
    };

    class PropertyElement : public Property{
    public:
        Data* data;
    public:
        ~PropertyElement() override;
        void onReduced(Production *p, Property **properties) override;
    private:
        void reduce0(PropertyTerminal* p0); //element → string
        void reduce1(PropertyTerminal* p0); //element → int
        void reduce2(PropertyTerminal* p0); //element → float
        void reduce3(PropertyTerminal* p0); //element → $true
        void reduce4(PropertyTerminal* p0); //element → $false
        void reduce5(PropertyTerminal* p0,PropertyArr* p1,PropertyTerminal* p2); //element → [ arr ]
        void reduce6(PropertyTerminal* p0,PropertyCompound* p1,PropertyTerminal* p2); //element → { compound }
    };
}



#endif
