//
// Created by stupid_coder_jyy on 2023/12/28.
//

#include "NBTUtil.h"
#include "StreamByteReader.h"
#include "StreamByteWriter.h"
#include "Error.h"

USING_LR

NBT *NBTUtil::fromFile(const QString &file, bool isBinary) {
    if (isBinary) {
        auto* reader = new StreamByteReader(file);
        if (reader->readShort() != BINARY_NBT_MAGIC_NUMBER) {
            delete reader;
            throw Error("NBTUtil::fromFile invalid magic number");
        }
        return Data::parseData(reader)->asCompound();
    }
    return new NBT();
}

void NBTUtil::toFile(NBT *tag, const QString& file, bool isBinary) {
    if (!tag) {
        throw Error("NBTUtil::toFile null nbt");
    }
    if (isBinary) {
        auto* writer = new StreamByteWriter(file);
        writer->writeShort(BINARY_NBT_MAGIC_NUMBER);
        tag->serialize(writer);
        delete writer;
    }
}
