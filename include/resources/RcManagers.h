//
// Created by stupid_coder_jyy on 2024/2/14.
//

#ifndef LIFERHYTHM_RCMANAGERS_H
#define LIFERHYTHM_RCMANAGERS_H

#include "ResourceManager.h"
#include "WidgetFactory.h"
#include <QTextCharFormat>

typedef QMap<QString, QTextCharFormat> StyleGroup;

class ImageStorage : public MultiSourceResourceManager<QPixmap>{
    STATIC_DEFINE(ImageStorage, QPixmap)
};

class StyleGroupStorage : public MultiSourceResourceManager<StyleGroup>{
    STATIC_DEFINE(StyleGroupStorage, StyleGroup)
};

class WidgetFactoryStorage : public MultiSourceResourceManager<WidgetFactory>{
    STATIC_DEFINE(WidgetFactoryStorage, WidgetFactory)
public:
    static void parseAll();
};

#undef TYPE
#endif //LIFERHYTHM_RCMANAGERS_H
