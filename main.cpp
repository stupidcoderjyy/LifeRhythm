
#include "Namespaces.h"
#include "LifeRhythm.h"
#include "WidgetDataStorage.h"
#include "PeriodType.h"
#include "Styles.h"
#include "RcManagers.h"
#include "MainTab.h"
#include "LineEdit.h"
#include "ColorSelector.h"
#include <QTimer>

USING_LR

class TestWidget : public QWidget {
public:
    explicit TestWidget(QWidget* parent = nullptr) {
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        setFocus();
    }

    void focusInEvent(QFocusEvent *event) override {
        qDebug() << "focus in";
    }

    void focusOutEvent(QFocusEvent *event) override {
        qDebug() << "focus out";
    }
};

int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    auto cfg = lr.getConfig();
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([](){
        MainTab::mainInit();
    });
    lr.onPostInit([](){
        auto* wct = WidgetFactoryStorage::get("log:widget_create_type")->apply();
        LifeRhythm::generateTitledDialog("新建类型", wct);
    });
    return lr.launch();
}
