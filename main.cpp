
#include <QApplication>
#include "BuiltInImageStorage.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    auto* storage = BuiltInImageStorage::getInstance();
    storage->init();
    return QApplication::exec();
}
