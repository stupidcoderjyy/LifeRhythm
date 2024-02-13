//
// Created by stupid_coder_jyy on 2024/2/7.
//

#ifndef LIFERHYTHM_BUILTINIMAGESTORAGE_H
#define LIFERHYTHM_BUILTINIMAGESTORAGE_H

#include <QPixmap>
#include "ResourceManager.h"

class BuiltInImageStorage : public BuiltInResourceManager<QPixmap>{
private:
    static BuiltInImageStorage instance;
public:
    static BuiltInImageStorage* getInstance();
private:
    BuiltInImageStorage();
protected:
    QPixmap *load(const Identifier &loc, const QString &absolutePath) override;
};


#endif //LIFERHYTHM_BUILTINIMAGESTORAGE_H
