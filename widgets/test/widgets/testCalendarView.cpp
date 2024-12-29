#include <QDebug>
\
#include "LifeRhythm.h"
#include "SlotsWidget.h"

#include <QHBoxLayout>
#include <utility>
#include "CalendarView.h"
#include <QDebug>

USING_NAMESPACE(lr)

class TestFeedBack : public Message {
public:
    explicit TestFeedBack(Identifier loc): Message(std::move(loc)) {
    }
};

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onPostInit([] {
        auto d = new CalendarData("test");
        LifeRhythm::get()->getMessageHandler().registerListener(d->getSenderLoc(), [d](auto, auto) {
            LifeRhythm::get()->getMessageHandler().sendMessage(new TestFeedBack(d->getReceiverLoc()));
            return MessageListener::SUCCESS;
        });
        auto v = new CalendarView;
        v->setData(d);
        v->loadCalendar(QDate::currentDate());
        auto parent = new QWidget;
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);
        layout->addWidget(v);
        parent->show();
    });
    return lr.launch();
}
