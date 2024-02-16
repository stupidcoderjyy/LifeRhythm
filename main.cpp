
#include <QApplication>
#include "TextEdit.h"
#include <QStringBuilder>
#include "RcManagers.h"
#include "NBTHighlighter.h"
#include "Styles.h"
#include <QTextCodec>
#include <QTimer>
#include "TestWidget.h"
#include <QLabel>
#include "LifeRhythm.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LifeRhythm::launch();
    return QApplication::exec();
}
