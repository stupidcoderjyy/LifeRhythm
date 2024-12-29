//
// Created by JYY on 24-11-21.
//


#include <ListWidget.h>

#include "TiledListWidget.h"
#include "LifeRhythm.h"
#include <QLayout>

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
    explicit List(QWidget *parent) : TiledListWidget(parent) {
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
    lr.onPostInit([] {
        auto d = new ListData;
        for (int i = 0; i < 100; i++) {
            d->append(new TestData(i));
        }

        auto parent = new QWidget;
        auto layout = new QHBoxLayout(parent);
        parent->setLayout(layout);

        auto l1 = new List(parent);
        l1->setFixedSize(200, 200);
        l1->setSlotSize(50, 50);
        l1->setColumnCount(7);
        l1->setColumnPolicy(TiledListWidget::Fixed);
        l1->setData(d);

        auto l2 = new List(parent);
        l2->setFixedSize(200, 200);
        l2->setSlotSize(50, 50);
        l2->setColumnCount(7);
        l2->setData(d);

        auto l3 = new List(parent);
        l3->setMinimumSize(200, 200);
        l3->setSlotSize(50, 50);
        QTimer::singleShot(1000, [l3, d] {
            l3->setData(d);
        });

        layout->addWidget(l1);
        layout->addWidget(l2);
        layout->addWidget(l3);
        parent->show();
    });
    return lr.launch();
}
