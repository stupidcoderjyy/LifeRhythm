
#include "WidgetFactory.h"
#include "WidgetFactoryStorage.h"
#include "QApplication"
#include <QHBoxLayout>
#include "ImageStorage.h"
#include "BufferedInput.h"
#include "TabWidget.h"

void testNormal() {
    WidgetFactory::init();
    ImageStorage::getInstance()->init("lr");
    auto* storage = WidgetFactoryStorage::getInstance();
    storage->init("lr");
    auto* loader = storage->get("lr:tab");
    regClazz(loader, TabWidget);
    regClazz(loader, CloseButton);
    loader->parse();

    auto* root = new QWidget(nullptr);
    auto* layout = new QHBoxLayout(root);
    root->setFixedSize(400, 400);
    root->setLayout(layout);
    root->setObjectName("root");
    root->setStyleSheet("#root{ background-color:#000000; }");
    auto* widget = loader->apply(root);
    layout->addWidget(widget);
    root->setVisible(true);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    testNormal();
    return QApplication::exec();
}
