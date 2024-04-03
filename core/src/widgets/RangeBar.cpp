//
// Created by stupid_coder_jyy on 2024/2/23.
//

#include <QHBoxLayout>
#include "RangeBar.h"
#include "WidgetUtil.h"

BarData::BarData(): NestedListNode(), begin(), end() {
}

BarData::BarData(int begin, int end): NestedListNode(), begin(begin), end(end) {
}

BarItem::BarItem(QWidget *parent): Widget(parent), row(), begin(), end() {
    setStyleSheet(bg(Styles::GRAY_2->rgbHex));
}

void BarItem::syncDataToWidget() {
    setVisible(wData);
    if (!wData) {
        return;
    }
    auto* d = wData->cast<BarData>();
    if (row != d->row || begin != d->begin || end != d->end) {
        begin = d->begin;
        end = d->end;
        row = d->row;
        emit sigUpdateWidget();
    }
}

void BarItem::syncWidgetToData() {
    if (!wData) {
        return;
    }
    auto* d = wData->cast<BarData>();
    d->begin = begin;
    d->end = end;
    d->row = row;
}

void BarItem::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, &BarItem::syncDataToWidget);
}

AbstractBarContainer::AbstractBarContainer(QWidget *parent):
        Widget(parent),minVal(0), maxVal(99), vpp(1), maxVpp(10), minVpp(1),
        zoomEnabled(true), zoomStep(2), rangeWidgets() {
    setFixedSize(0, 0);
    setObjectName("rc");
    setStyleSheet(qss_target("rc", bg(Styles::BLACK->rgbHex)));
}

void AbstractBarContainer::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() != Qt::ControlModifier || !zoomEnabled) {
        return;
    }
    event->accept();
    if (event->angleDelta().y() > 0) {
        if (vpp == maxVpp) {
            return;
        }
        vpp = qMin(maxVpp, vpp + zoomStep);
    } else  {
        if (vpp == minVpp) {
            return;
        }
        vpp = qMax(minVpp, vpp - zoomStep);
    }
    emit sigZoom();
}

VBarContainer::VBarContainer(QWidget *parent): AbstractBarContainer(parent) {
}

void VBarContainer::updateRangeWidgetGeometry(BarItem* rw) {
    int y = qRound(rw->begin * vpp);
    int height = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(0, y, rect().width(), height);
}

void VBarContainer::updateBarGeometry() {
    int height = qRound((maxVal - minVal) * vpp);
    setFixedSize(parentWidget()->width(), height);
    for (auto* rw : rangeWidgets) {
        updateRangeWidgetGeometry(rw);
    }
}

HBarContainer::HBarContainer(QWidget *parent): AbstractBarContainer(parent) {
}

void HBarContainer::updateRangeWidgetGeometry(BarItem *rw) {
    int x = qRound(rw->begin * vpp);
    int width = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(x, 0, width, rect().height());
}

void HBarContainer::updateBarGeometry() {
    int width = qRound((maxVal - minVal) * vpp);
    setFixedSize(width, parentWidget()->height());
    for (auto* rw : rangeWidgets) {
        updateRangeWidgetGeometry(rw);
    }
}

RangeBar::RangeBar(AbstractBarContainer* c, QWidget *parent): ScrollArea(parent) {
    assembled = false;
    rootContent = new QWidget(this);
    rootContent->setObjectName("root");
    rootContent->setStyleSheet(qss_target("root", bg(Styles::CLEAR->rgbHex)));
    setWidget(rootContent);
    if (!c) {
        throwInFunc("null container");
    }
    if (dynamic_cast<VBarContainer*>(c)) {
        isVertical = true;
    } else if (dynamic_cast<HBarContainer*>(c)) {
        isVertical = false;
    } else {
        throwInFunc("do not directly override AbstractRangeWidgetsContainer");
    }
    connect(c, &AbstractBarContainer::sigZoom, this, [this](){
        barDataChanged(Zoom);
    });
    c->setParent(rootContent);
    container = c;
}

void RangeBar::syncDataToWidget() {
    auto* ld = wData->cast<ListData>();
    int j = ld->getChangeEnd();
    int size = container->rangeWidgets.length();
    while (size++ <= j) {
        auto* rw = createRangeWidget();
        initPeriodWidget(rw);
        container->rangeWidgets << rw;
    }
    for (int i = ld->getChangeBegin(); i <= j; i++) {
        auto* rw = container->rangeWidgets.at(i);
        rw->setData(ld->at(i));
        rw->syncDataToWidget();
    }
}

void RangeBar::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this](){
        syncDataToWidget();
    });
}

void RangeBar::resizeEvent(QResizeEvent *event) {
    ScrollArea::resizeEvent(event);
    if (!assembled) {
        assembleContainer();
        barDataChanged(Content);
        assembled = true;
    }
}

BarItem *RangeBar::createRangeWidget() {
    return new BarItem();
}

void RangeBar::assembleContainer() {
    auto* l = new QHBoxLayout(rootContent);
    l->setContentsMargins({});
    rootContent->setLayout(l);
    l->addWidget(container);
}

void RangeBar::updateContainerSize() {
    rootContent->setFixedSize(container->size());
}

void RangeBar::initPeriodWidget(BarItem* rw) {
    connect(rw, &BarItem::sigUpdateWidget, [this, rw](){
        container->updateRangeWidgetGeometry(rw);
    });
    rw->setParent(container);
}

void RangeBar::barDataChanged(BarUpdateType type) {
    if (type == Content) {
        container->updateBarGeometry();
    } else if (isVertical) {
        performVerticalZoom();
    } else {
        performHorizontalZoom();
    }
    updateContainerSize();
    emit sigBarLayoutChanged();
}

void RangeBar::performHorizontalZoom() {
    auto* bar = horizontalScrollBar();
    auto dx = (QCursor::pos() - getGlobalPos(this)).x();
    double dw = qMax(0, qMin(width(), dx));
    double ratio = ((double)bar->value() / bar->maximum() * (container->width() - viewport()->width()) + dw) / container->width();
    container->updateBarGeometry();
    bar->setMaximum(container->width() - viewport()->width());
    bar->setValue(qRound(ratio * container->width() - dw));
}

void RangeBar::performVerticalZoom() {
    auto* bar = verticalScrollBar();
    auto dy = (QCursor::pos() - getGlobalPos(this)).y();
    double dh = qMax(0, qMin(height(), dy));
    double ratio = ((double)bar->value() / bar->maximum() * (container->height() - viewport()->height()) + dh) / container->height();
    container->updateBarGeometry();
    bar->setMaximum(container->height() - viewport()->height());
    bar->setValue(qRound(ratio * container->height() - dh));
}
