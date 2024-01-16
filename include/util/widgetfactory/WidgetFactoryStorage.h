//
// Created by stupid_coder_jyy on 2024/1/14.
//

#ifndef LIFERHYTHM_WIDGETFACTORYSTORAGE_H
#define LIFERHYTHM_WIDGETFACTORYSTORAGE_H

#include "ResourceManager.h"
#include "WidgetFactory.h"

class WidgetFactoryStorage : public ResourceManager<WidgetFactory> {
private:
    static WidgetFactoryStorage instance;
public:
    static WidgetFactoryStorage* getInstance();
private:
    WidgetFactoryStorage();
    WidgetFactory *load(const Identifier &loc) override;
    void loadFailure(std::exception &e) noexcept override;
};

#endif //LIFERHYTHM_WIDGETFACTORYSTORAGE_H
