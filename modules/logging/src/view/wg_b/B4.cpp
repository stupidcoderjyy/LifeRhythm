//
// Created by stupid_coder_jyy on 2024/4/15.
//

#include "B4.h"
#include "RcManagers.h"
#include <QHBoxLayout>

USING_NAMESPACE(lr::log)

WeekViewContainer::WeekViewContainer(QWidget *parent): VBarContainer(parent) {
}

void WeekViewContainer::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPen pen;
    pen.setColor(Styles::GRAY_1->color);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawRect(rect());
    pen.setWidth(1);
    painter.setPen(pen);
    int ly;
    for (int t = minVal; t <= maxVal; t += 60) {
        ly = qRound(t * vpp);
        painter.drawLine(0, ly, width(), ly);
    }
    for (int i = 0, x = sideLen; i < 6; i ++, x += sideLen) {
        painter.drawLine(x, 0, x, height());
    }
}

B4::B4(QWidget *parent): RangeBar(new WeekViewContainer, parent) {
}

void B4::assembleContainer() {
    auto* l = new QHBoxLayout(rootContent);
    rootContent->setLayout(l);
    scale = new TimeScale(rootContent);
    connect(this, &B4::sigBarLayoutChanged, scale, [this](){
        scale->setVpp(container->getVpp());
    });
    l->setContentsMargins(0,0,0,0);
    l->setSpacing(5);
    l->addWidget(scale);
    l->addWidget(container);
}

void B4::updateContentSize() {
    scale->setFixedHeight(container->height());
    rootContent->updateGeometry();
}

BarItem *B4::createRangeWidget() {
    return WidgetFactoryStorage::get("log:item_timebar")->applyAndCast<BarItem>();
}

ScrollBar *B4::createVerticalScrollBar() {
    auto* bar = ScrollArea::createVerticalScrollBar();
    bar->setEnabled(false);
    return bar;
}
