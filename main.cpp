
#include <Error.h>
#include "NBT.h"
#include "SyntaxAnalyzer.h"
#include "CompileError.h"
#include "PrintErrorHandler.h"
#include "CompilerInput.h"
#include "Lexer.h"

int main(int argc, char *argv[]) {
    SyntaxAnalyzer* analyzer;
    NBT* nbt = nullptr;
    try {
        analyzer = new SyntaxAnalyzer("test.txt");
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
