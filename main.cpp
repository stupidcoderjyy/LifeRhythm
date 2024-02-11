
#include "LifeRhythm.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LifeRhythm::launch();
    return QApplication::exec();
}
