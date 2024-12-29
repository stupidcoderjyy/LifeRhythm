#include <qboxlayout.h>
#include <qthread.h>
#include <RcManagers.h>
#include "LifeRhythm.h"
#include "SlotsPainter.h"
#include "ListWidget.h"
#include "Calendar.h"
#include "DropDown.h"

USING_NAMESPACE(lr)
USING_NAMESPACE(lr::calendar)

class TestItem : public ListItemCalendar {
public:
    void paintEvent(QPaintEvent *event) override {
        if (wData) {
            QPainter painter(this);
            painter.fillRect(event->rect(), Styles::GRAY_0->color);
        }
        ListItemCalendar::paintEvent(event);
    }
};

class TestCalendarData : public CalendarData {
protected:
    WidgetData *getData(const QDate &date) const override {
        if (date.dayOfWeek() > Qt::Friday) {
            return new WidgetData;
        }
        return nullptr;
    }
};

class TestCalendar : public Calendar {
public:
    explicit TestCalendar(QWidget *parent = nullptr, bool iic = true) : Calendar(parent, iic) {}
protected:
    ListItemCalendar *newItem() override {
        return new TestItem;
    }
};

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([] {
        Button::mainInit();
        Calendar::mainInit();
    });
    lr.onPostInit([] {
        auto parent = new QWidget;
        parent->setObjectName("parent");
        parent->setStyleSheet(qss_target("#parent", bg(Styles::BLACK->rgbHex)));
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);
        auto* c = new TestCalendar;
        c->setData(new TestCalendarData);
        layout->addWidget(c);
        QTimer::singleShot(0, [parent] {
            parent->show();
        });
    });
    return lr.launch();
}
