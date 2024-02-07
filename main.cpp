
#include "WidgetFactory.h"
#include "WidgetFactoryStorage.h"
#include "QApplication"
#include "QssParser.h"
#include <QHBoxLayout>
#include "ImageStorage.h"

void testNormal() {
    WidgetFactory::init();
    auto* storage = WidgetFactoryStorage::getInstance();
    storage->init("lr");
    auto* loader = storage->get("lr:a");
    loader->parse();

    auto* root = new QWidget(nullptr);
    auto* layout = new QHBoxLayout(root);
    root->setFixedSize(400, 400);
    root->setLayout(layout);
    root->setObjectName("root");
    root->setStyleSheet("#root{ background-color:#000000; }");
    auto* widget = loader->apply(root);
    widget->setFixedSize(200,200);
    layout->addWidget(widget);
    root->setVisible(true);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ImageStorage::getInstance()->init("lr");
    return QApplication::exec();
}
