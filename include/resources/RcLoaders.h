//
// Created by stupid_coder_jyy on 2024/2/14.
//

#ifndef LIFERHYTHM_RCLOADERS_H
#define LIFERHYTHM_RCLOADERS_H

#include "RcLoader.h"
#include "WidgetFactory.h"
#include "Plugin.h"
#include <QTextCharFormat>

class CORE_API ImageRcLoader final : public RcLoader<QPixmap> {
public:
    QPixmap *load(const Identifier &loc, const QString &absolutePath) override;
SINGLETON_HEADER(ImageRcLoader)
};

class CORE_API WidgetFactoryRcLoader final : public RcLoader<WidgetFactory> {
public:
    WidgetFactory *load(const Identifier &loc, const QString &absolutePath) override;
    void onLoadFailed(std::exception &e) noexcept override;
SINGLETON_HEADER(WidgetFactoryRcLoader)
};

class CORE_API StyleGroupRcLoader final : public RcLoader<QMap<QString, QTextCharFormat>> {
public:
    QMap<QString, QTextCharFormat> *load(const Identifier &loc, const QString &absolutePath) override;
SINGLETON_HEADER(StyleGroupRcLoader)
};


#endif //LIFERHYTHM_RCLOADERS_H
