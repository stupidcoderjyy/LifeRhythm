//
// Created by stupid_coder_jyy on 2024/2/14.
//

#include "RcLoaders.h"
#include "NBT.h"
#include "CompileError.h"
#include "StyleParser.h"
#include "Styles.h"
#include <QPixmap>

QPixmap *ImageRcLoader::load(const Identifier &loc, const QString &absolutePath) {
    return new QPixmap(absolutePath);
}

WidgetFactory *WidgetFactoryRcLoader::load(const Identifier &loc, const QString &absolutePath) {
    auto* nbt = NBT::fromStringNbt(absolutePath);
    auto name = loc.getPath();
    if (const int i = name.lastIndexOf('/'); i > 0) {
        name = name.mid(i + 1, name.length() - i - 1);
    }
    return WidgetFactory::fromNbt(name, nbt);
}

void WidgetFactoryRcLoader::onLoadFailed(std::exception &e) noexcept {
    static_cast<CompileError&>(e).printErr();
}

QMap<QString, QTextCharFormat> *StyleGroupRcLoader::load(const Identifier &loc, const QString &absolutePath) {
    auto* map = new QMap<QString, QTextCharFormat>();
    NBT* nbt;
    try {
        nbt = NBT::fromStringNbt(absolutePath);
    } catch (CompileError& err) {
        PrintCompileErrorHandler().onErrorCaught(err);
        return map;
    }
    auto it = nbt->get().begin();
    while (it != nbt->get().end()) {
        if (auto* child = it.value(); child->type == Data::COMPOUND) {
            QTextCharFormat fmt = Styles::FORMAT_DEFAULT;
            highlight::StyleParser::parse(&fmt, child->asCompound());
            map->insert(it.key(), fmt);
        }
        ++it;
    }
    delete nbt;
    return map;
}
