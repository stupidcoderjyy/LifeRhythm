
#include <QApplication>
#include "LifeRhythm.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LifeRhythm::launch();
    return QApplication::exec();
}
