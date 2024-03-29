//
// Created by stupid_coder_jyy on 2023/12/28.
//

#ifndef LIFERHYTHM_NBTUTIL_H
#define LIFERHYTHM_NBTUTIL_H

#include "NBT.h"

class NBTUtil {
public:
    static const short STRING_NBT_MAGIC_NUMBER = 0x1232;
    static const short BINARY_NBT_MAGIC_NUMBER = 0x4565;
    static NBT* fromFile(const QString& file, bool isBinary = true);
    static void toFile(NBT* tag, const QString& file, bool isBinary = true);
};
#endif //LIFERHYTHM_NBTUTIL_H
