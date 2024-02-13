
#include <QApplication>
#include "CompilerInput.h"
#include "Lexer.h"
#include "NBT.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    auto* nbt = NBT::fromStringNbt(":/assets/lr/factories/tab.snbt");
    qDebug() << nbt->toString();
    return QApplication::exec();
}
