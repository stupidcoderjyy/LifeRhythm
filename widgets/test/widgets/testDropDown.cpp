//
// Created by JYY on 24-11-21.
//


#include <ListWidget.h>

#include "TiledListWidget.h"
#include "LifeRhythm.h"
#include <QLayout>
#include <RcManagers.h>

#include "Button.h"
#include "DropDown.h"
#include "Label.h"

USING_NAMESPACE(lr)

class TestData : public WidgetData {
public:
    int data;

    explicit TestData(int data) : data(data) {
    }
};

class TestItem : public ListItem {
private:
    int i;
    int d;

public:
    explicit TestItem(int idx, QWidget *parent) : ListItem(parent), i(idx), d(-1) {
    }

    void syncDataToWidget() override {
        if (wData) {
            d = wData->cast<TestData>()->data;
        } else {
            d = -1;
        }
    }

    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.fillRect(rect(), Styles::GRAY_0->color);
        painter.setFont(Styles::FONT_MAIN);
        painter.setPen(Styles::GRAY_TEXT_0->color);
        painter.drawText(rect(), QString::asprintf("i:%d\nd:%d", i, d), QTextOption(Qt::AlignCenter));
    }
};

class List : public TiledListWidget {
public:
    explicit List(QWidget *parent, bool i = true) : TiledListWidget(parent, i) {
    }

    ListItem *newItem() override {
        return new TestItem(items.length(), this);
    }
};


int main(int argc, char *argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([] {
        Button::mainInit();
        auto f = WidgetFactoryStorage::get("test:test_drop_down");
        regClazz(f, DropDown);
        regClazz(f, Label);
        regClazz(f, List);
        regClazz(f, Widget);
    });
    lr.onPostInit([] {
        auto d = new ListData;
        for (int i = 0; i < 100; i++) {
            d->append(new TestData(i));
        }

        auto dp1 = WidgetFactoryStorage::get("test:test_drop_down")->applyAndCast<DropDown>();
        auto dp2 = new DropDown();
        dp2->setStyle(DropDown::Jetbrains);
        dp2->setFixedSize(200,50);

        auto l = dp1->getPointer<List>("l");
        l->setFixedSize(200, 200);
        l->setSlotSize(50, 50);
        l->setColumnCount(7);
        l->setColumnPolicy(TiledListWidget::Fixed);
        l->setData(d);

        auto parent = new QWidget;
        parent->setObjectName("p");
        parent->setStyleSheet(qss_target("#p", bg(Styles::BLACK->rgbHex)));
        parent->setFixedSize(500, 500);
        auto layout = new QHBoxLayout(parent);
        parent->setLayout(layout);

        layout->addWidget(dp1);
        layout->addWidget(dp2);
        parent->show();
    });
    return lr.launch();
}
