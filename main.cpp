
#include "QDebug"
#include "RangeBar.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    RangeBar bar(true);
    bar.setBarRange(0, 1000);
    bar.setVpp(1);
    bar.setZoomEnabled();
    bar.setZoomRange(1, 20);
    bar.setZoomStep(1);
    bar.setFixedSize(200, 400);
    for (int i = 0 ; i <= 1000 ; i += 100) {
        bar.addPeriod(i, i + 20);
    }
    bar.show();
    return QApplication::exec();
}
