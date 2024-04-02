//
// Created by stupid_coder_jyy on 2024/3/2.
//

#include "TimeBar.h"
#include "RcManagers.h"
#include "ListData.h"
#include "Period.h"
#include <QHBoxLayout>
#include <QPainterPath>

USING_NAMESPACE(lr::log)

TimeBarItem::TimeBarItem(QWidget *parent): RangeBarItem(parent), labelInfo(), labelRange() {
    colorBg = Styles::GRAY_1->color;
}

void TimeBarItem::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<TimeBarItem*>(target)->init();
    };
}

void TimeBarItem::init() {
    labelInfo = getPointer<TextLabel>("labelInfo");
    labelRange = getPointer<TextLabel>("labelRange");
}

void TimeBarItem::syncDataToWidget() {
    RangeBarItem::syncDataToWidget();
    if (wData) {
        show();
        auto* d = wData->cast<Period>();
        labelInfo->setText(d->info);
        labelRange->setText(QString::asprintf("%02d:%02d-%02d:%02d", d->begin / 60, d->begin % 60, d->end / 60, d->end % 60));
    } else {
        close();
    }
}

void TimeBarItem::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(QRect(4, 2, width() - 8, height() - 4), 3, 3);
    p.fillPath(path, colorBg);
}

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

void TimeBarContainer::updateBarGeometry() {
    int height = qRound((maxVal - minVal) * vpp);
    setFixedSize(parentWidget()->width() - 55, height); // scale 50, spacing 5
    for (auto* rw : rangeWidgets) {
        updateRangeWidgetGeometry(rw);
    }
}

TimeBar::TimeBar(QWidget *parent): RangeBar(new TimeBarContainer(), parent) {
    setBarRange(0, 1440);
    setZoomRange(0.8, 2.4);
    setZoomStep(0.4);
    setVpp(1.2);
}

ScrollBar *TimeBar::createVerticalScrollBar() {
    auto* bar = ScrollArea::createVerticalScrollBar();
    bar->setVisible(false);
    return bar;
}

RangeBarItem *TimeBar::createRangeWidget() {
    return WidgetFactoryStorage::get("log:item_timebar")->applyAndCast<RangeBarItem>();
}

void TimeBar::assembleContainer() {
    auto* l = new QHBoxLayout(rootContent);
    rootContent->setLayout(l);
    scale = new TimeScale(rootContent);
    connect(this, &RangeBar::sigBarDataChanged, scale, &TimeScale::setVpp);
    l->setContentsMargins(0,0,0,0);
    l->setSpacing(5);
    l->addWidget(scale);
    l->addWidget(container);
}

void TimeBar::updateContainerSize() {
    scale->setFixedHeight(container->height());
    rootContent->updateGeometry();
}

TimeScale::TimeScale(QWidget *parent): QWidget(parent), vpp(-1) {
    setFixedWidth(50);
}

void TimeScale::setVpp(double v3) {
    vpp = v3;
    update();
}

void TimeScale::paintEvent(QPaintEvent *event) {
    if (vpp < 0) {
        return;
    }
    QPainter p(this);
    QPen pen;
    pen.setColor(Styles::GRAY_3->color);
    p.setPen(pen);
    p.setFont(Styles::FONT_SMALL);
    for (int hour = 1 ; hour < 24 ; hour ++) {
        QString str = hour == 12 ? "正午" : QString::number(hour).append(":00");
        QRect r(0, qRound(hour * 60 * vpp) - 20, width(), 40);
        p.drawText(r, Qt::AlignVCenter | Qt::AlignRight, str);
    }
}