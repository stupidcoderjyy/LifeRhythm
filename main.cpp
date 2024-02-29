

#include <QLabel>
#include <utility>
#include "SelectableListWidget.h"
#include "OptionsBox.h"
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "TabBar.h"

USING_LR

class TestTabWidget : public TabWidget {
public:
    TestTabWidget(const QString& color): TabWidget() {
        setStyleSheet(bg(color));
    }
};

int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    auto cfg = lr.getConfig();
    cfg.setMode(Config::Normal);
    lr.setConfig(cfg);
    lr.onPostInit([](){
        LifeRhythm::insertTab("a", new TestTabWidget(Styles::CYAN_0));
        LifeRhythm::insertTab("b", new TestTabWidget(Styles::GOLD));
        LifeRhythm::insertTab("c", new TestTabWidget(Styles::GRAY_2));
    });
    return lr.launch();
}
