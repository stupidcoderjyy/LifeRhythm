
#include <QApplication>
#include "TextEdit.h"
#include <QStringBuilder>
#include "RcManagers.h"
#include "NBTHighlighter.h"
#include "Styles.h"
#include <QTextCodec>
#include <QTimer>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    Styles::initStyles();
    StyleGroupStorage::init();
    while (true) {
        TextEdit edit(nullptr);
    }

//    auto* edit = new TextEdit(nullptr);
//    new highlight::NBTHighlighter(new highlight::NBTContext(), new highlight::NBTLexer(), edit->document());
//    edit->setMinimumSize(500,500);
//    edit->insertPlainText("a: 10\n"
//                          "b: 2.5f\n"
//                          "c: \"aaa\"\n"
//                          "d: true\n"
//                          "e: [0,1,2]\n"
//                          "//dwadwadwadwa\n"
//                          "/*\n"
//                          "dawdawda\n"
//                          "dawdadaw\n"
//                          "*/\n"
//                          "f: 达瓦达瓦");
//    edit->show();
    return QApplication::exec();
}
