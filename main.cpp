#include <QDebug>
#include <Error.h>
#include <PrintErrorHandler.h>
#include "NBTUtil.h"

int main(int argc, char *argv[]) {
    PrintErrorHandler errHandler;
    try {
        auto* nbt = new NBT();
        nbt->putInt("i")->setVal(1);
        nbt->putArray("arr")->addInt(1)->addInt(2)->addInt(3);
        nbt->putCompound("tag")->putInt("ii")->setVal(2);
        NBTUtil::toFile(nbt, "test.dat");
        delete nbt;
        nbt = NBTUtil::fromFile("test.dat");
        qDebug() << nbt->toString();
    } catch (Error& err) {
        errHandler.onErrorCaught(err);
    }
    return 0;
}
