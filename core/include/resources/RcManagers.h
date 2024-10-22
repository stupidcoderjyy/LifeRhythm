//
// Created by stupid_coder_jyy on 2024/2/14.
//

#ifndef LIFERHYTHM_RCMANAGERS_H
#define LIFERHYTHM_RCMANAGERS_H

#include "ResourceManager.h"
#include "WidgetFactory.h"
#include "Plugin.h"
#include <QTextCharFormat>

typedef QMap<QString, QTextCharFormat> StyleGroup;

class CORE_API ImageStorage : public MultiSourceResourceManager<QPixmap>{
    STATIC_DEFINE(ImageStorage, QPixmap)
};

class CORE_API StyleGroupStorage : public MultiSourceResourceManager<StyleGroup>{
    STATIC_DEFINE(StyleGroupStorage, StyleGroup)
};

class CORE_API WidgetFactoryStorage : public MultiSourceResourceManager<WidgetFactory>{
    STATIC_DEFINE(WidgetFactoryStorage, WidgetFactory)
public:
    static void parseAll();
};

#endif //LIFERHYTHM_RCMANAGERS_H
