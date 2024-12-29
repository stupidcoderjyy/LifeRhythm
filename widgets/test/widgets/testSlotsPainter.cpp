#include <QDebug>
\
#include "LifeRhythm.h"
#include "SlotsPainter.h"

USING_NAMESPACE(lr)

#include <QHBoxLayout>
#include "ListWidget.h"
#include <QDebug>

class TestLayer : public SlotsPainterLayer {
public:
    TestLayer(): SlotsPainterLayer(1) {}
protected:
    void beforeDrawing(QPainter &p) override {
        p.setFont(Styles::FONT_LARGE);
        p.setPen(Styles::GRAY_TEXT_0->color);
    }

    void drawSlot(QPainter &p, QRect &area, int row, int column) override {
        p.drawText(area, QString::number(row) + "," + QString::number(column), QTextOption(Qt::AlignCenter));
    }
};

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onPostInit([] {
        auto parent = new QWidget;
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);

        auto sw = new SlotsPainter(parent);
        sw->setSlotCount(5, 3);
        sw->setMinimumSize(200, 200);
        sw->setSizePolicy(QSizePolicy:: Minimum, QSizePolicy::Minimum);
        sw->addLayer(new TestLayer);
        sw->addLayer(new sp_layers::BackgroundLayer);
        layout->addWidget(sw);
        parent->show();
    });
    return lr.launch();
}
