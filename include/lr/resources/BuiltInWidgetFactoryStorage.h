//
// Created by stupid_coder_jyy on 2024/1/14.
//

#ifndef LIFERHYTHM_BUILTINWIDGETFACTORYSTORAGE_H
#define LIFERHYTHM_BUILTINWIDGETFACTORYSTORAGE_H

#include "ResourceManager.h"
#include "WidgetFactory.h"

class BuiltInWidgetFactoryStorage : public BuiltInResourceManager<WidgetFactory> {
private:
    static BuiltInWidgetFactoryStorage instance;
public:
    static BuiltInWidgetFactoryStorage* getInstance();
    void parseAll();
protected:
    BuiltInWidgetFactoryStorage();
    WidgetFactory *load(const Identifier &loc, const QString &absolutePath) override;
    void loadFailure(std::exception &e) noexcept override;
};

#endif //LIFERHYTHM_BUILTINWIDGETFACTORYSTORAGE_H
