
#include "WidgetFactory.h"
#include "WidgetFactoryStorage.h"
#include "QApplication"
#include "StdWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    WidgetFactory::init();
    auto* storage = WidgetFactoryStorage::getInstance();
    while (true) {
        storage->init("lr");
        auto* loader = storage->get("lr:a");
        if (loader) {
            loader->parse();
            auto* widget = new QWidget(nullptr);
            loader->apply(nullptr, widget);
            delete widget;
        }
        storage->unload("lr");
    }
    return QApplication::exec();
}
