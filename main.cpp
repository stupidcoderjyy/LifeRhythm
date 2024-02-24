
#include "QDebug"
#include "TimeBar.h"
#include <QApplication>
#include "Error.h"
#include "CompileError.h"
#include "PrintErrorHandler.h"
#include "ElementsManager.h"
#include <QTextCodec>
#include <QTimer>
#include <QDebug>

#define pTime(h, m) (h * 60 + m)

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    try {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        TimeBar::init();
        TimeBar bar;
        bar.setFixedSize(200, 400);
        ListElementsManager<Period> manager("", "test.dat");
//        manager.add(new Period(pTime(1,20), pTime(2,30), "abcd"));
//        manager.add(new Period(pTime(3,0), pTime(3,20), "达瓦达瓦达瓦"));
//        manager.save();
        manager.load();
        for (Period* p : manager.elements()) {
            bar.addPeriod(p);
        }
        bar.show();
        return QApplication::exec();
    } catch (Error& err) {
        PrintErrorHandler().onErrorCaught(err);
    } catch (CompileError& err) {
        PrintCompileErrorHandler().onErrorCaught(err);
    }
    return -1;
}
