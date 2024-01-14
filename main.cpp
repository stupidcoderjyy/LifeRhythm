
#include <Error.h>
#include "NBT.h"
#include "StringNbtParser.h"
#include "CompileError.h"
#include "PrintErrorHandler.h"
#include "CompilerInput.h"
#include "StringNbtLexer.h"

int main(int argc, char *argv[]) {
    StringNbtParser* analyzer;
    NBT* nbt = nullptr;
    try {
        analyzer = new StringNbtParser("test.txt");
        nbt = analyzer->run();
        qDebug() << nbt->toString();
    } catch (CompileError& err) {
        PrintCompileErrorHandler().onErrorCaught(err);
    } catch (Error& err) {
        PrintErrorHandler().onErrorCaught(err);
    }
    delete analyzer;
    delete nbt;
}
