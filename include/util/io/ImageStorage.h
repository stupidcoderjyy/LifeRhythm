//
// Created by stupid_coder_jyy on 2024/2/7.
//

#ifndef LIFERHYTHM_IMAGESTORAGE_H
#define LIFERHYTHM_IMAGESTORAGE_H

#include <QPixmap>
#include "ResourceManager.h"

class ImageStorage : public ResourceManager<QPixmap>{
private:
    static ImageStorage instance;
public:
    static ImageStorage* getInstance();
private:
    ImageStorage();
    QPixmap *load(const Identifier &loc) override;
};


#endif //LIFERHYTHM_IMAGESTORAGE_H
