//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "TimeBar.h"
#include "RcManagers.h"
#include <QHBoxLayout>

USING_NAMESPACE(lr::log)

TimeBarContainer::TimeBarContainer(QWidget *parent): VBarContainer(parent) {
}

void TimeBarContainer::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPen pen;
    pen.setColor(Styles::GRAY_1->color);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawRect(rect());
    pen.setWidth(1);
    painter.setPen(pen);
    int ly;
    for (int t = minVal ; t <= maxVal ; t += 60) {
        ly = qRound(t * vpp);
        painter.drawLine(0, ly, width(), ly);
    }
}

TimeBar::TimeBar(QWidget *parent): RangeBar(new TimeBarContainer(), parent), scale() {
}

ScrollBar *TimeBar::createVerticalScrollBar() {
    auto* bar = ScrollArea::createVerticalScrollBar();
    bar->setEnabled(false);
    return bar;
}

BarItem *TimeBar::createRangeWidget() {
    return WidgetFactoryStorage::get("log:item_timebar")->applyAndCast<BarItem>();
}

void TimeBar::assembleContainer() {
    auto* l = new QHBoxLayout(rootContent);
    rootContent->setLayout(l);
    scale = new TimeScale(rootContent);
    connect(this, &RangeBar::sigBarLayoutChanged, scale, [this](){
        scale->setVpp(container->getVpp());
    });
    l->setContentsMargins(0,0,0,0);
    l->setSpacing(5);
    l->addWidget(scale);
    l->addWidget(container);
}

void TimeBar::updateContainerSize() {
    scale->setFixedHeight(container->height());
    rootContent->updateGeometry();
}