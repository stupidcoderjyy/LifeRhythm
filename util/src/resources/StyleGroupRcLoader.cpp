//
// Created by JYY on 24-12-24.
//

#include "StyleGroupRcLoader.h"
#include "NBT.h"
#include "CompileError.h"
#include "StyleParser.h"
#include "Styles.h"

USING_LR

QMap<QString, QTextCharFormat> *StyleGroupRcLoader::load(const Identifier &loc, const QString &absolutePath) {
    auto *map = new QMap<QString, QTextCharFormat>();
    NBT *nbt;
    try {
        nbt = NBT::fromStringNbt(absolutePath);
    } catch (CompileError &err) {
        PrintCompileErrorHandler().onErrorCaught(err);
        return map;
    }
    auto it = nbt->get().begin();
    while (it != nbt->get().end()) {
        if (auto *child = it.value(); child->type == Data::COMPOUND) {
            QTextCharFormat fmt = Styles::FORMAT_DEFAULT;
            highlight::StyleParser::parse(&fmt, child->asCompound());
            map->insert(it.key(), fmt);
        }
        ++it;
    }
    delete nbt;
    return map;
}