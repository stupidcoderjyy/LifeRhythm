
#include "LifeRhythm.h"
#include <QApplication>

#include <QLabel>
#include "ScrollArea.h"
#include "Styles.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
//    LifeRhythm::launch();
    ScrollArea area;
    QLabel label(&area);
    label.setStyleSheet(bg(Styles::GRAY_0));
    QFont font = Styles::MAIN_FONT;
    font.setPointSize(500);
    label.setFont(font);
    label.setText("Hello World");
    area.setWidget(&label);
    area.setFixedSize(500,500);
    area.show();
    return QApplication::exec();
}
