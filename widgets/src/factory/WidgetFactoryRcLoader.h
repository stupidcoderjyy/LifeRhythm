//
// Created by JYY on 24-12-24.
//

#ifndef WIDGETFACTORYRCLOADER_H
#define WIDGETFACTORYRCLOADER_H

#include "Namespaces.h"
#include "RcLoader.h"
#include "WidgetFactory.h"

BEGIN_LR

class CORE_API WidgetFactoryRcLoader final : public RcLoader<WidgetFactory> {
public:
    WidgetFactory *load(const Identifier &loc, const QString &absolutePath) override;
    void onLoadFailed(std::exception &e) noexcept override;
SINGLETON_HEADER(WidgetFactoryRcLoader)
};

END_LR


#endif //WIDGETFACTORYRCLOADER_H
