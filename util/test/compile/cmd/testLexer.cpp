//
// Created by JYY on 25-1-1.
//

#include <QApplication>
#include <QHBoxLayout>

#include "CmdHighlighter.h"
#include "Namespaces.h"
#include "QTextEdit"
#include "StyleGroupStorage.h"
#include "Styles.h"

USING_NP(lr::cmd)
USING_LR

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    Styles::initStyles();
    StyleGroupStorage::init();

    auto parent = new QWidget;
    parent->setFixedSize(500, 500);
    parent->setObjectName("parent");
    parent->setStyleSheet(qss_target("#parent", bg(Styles::BLACK->rgbHex)));
    auto layout = new QHBoxLayout(parent);
    parent->setLayout(layout);

    auto edit = new QTextEdit(parent);
    edit->setFrameStyle(QFrame::NoFrame);
    edit->setContextMenuPolicy(Qt::NoContextMenu);
    edit->setFont(Styles::FONT_MAIN);
    edit->setObjectName("te");
    edit->setStyleSheet(qss_object("te", bg(Styles::CLEAR->rgbHex)));

    layout->addWidget(edit);
    CmdHighlighter highlighter(LOC("lr:cmd"), edit->document());

    parent->show();
    return QApplication::exec();
}