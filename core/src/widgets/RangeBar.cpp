//
// Created by stupid_coder_jyy on 2024/2/23.
//

#include <QHBoxLayout>
#include "RangeBar.h"
#include "Error.h"

RangeBarData::RangeBarData(): WidgetData(), begin(), end() {
}

int RangeBarData::getBegin() const {
    return begin;
}

int RangeBarData::getEnd() const {
    return end;
}

void RangeBarData::setBegin(int b) {
    begin = b;
}

void RangeBarData::setEnd(int e) {
    end = e;
}

AbstractRangeWidgetsContainer::AbstractRangeWidgetsContainer(QWidget *parent):
        Widget(parent),minVal(0), maxVal(99), vpp(1), maxVpp(10), minVpp(1),
        zoomEnabled(true), zoomStep(2), rangeWidgets() {
    setFixedSize(0, 0);
    setObjectName("rc");
    setStyleSheet(qss_target("rc", bg(Styles::BLACK->rgbHex)));
}

void AbstractRangeWidgetsContainer::wheelEvent(QWheelEvent *event) {
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

VRangeItemsContainer::VRangeItemsContainer(QWidget *parent): AbstractRangeWidgetsContainer(parent) {
}

void VRangeItemsContainer::updateRangeWidget(RangeBarItem* rw) {
    int y = qRound(rw->begin * vpp);
    int height = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(0, y, rect().width(), height);
}

void VRangeItemsContainer::updateBar() {
    int height = qRound((maxVal - minVal) * vpp);
    setFixedSize(parentWidget()->width(), height);
    for (auto* rw : rangeWidgets) {
        updateRangeWidget(rw);
    }
}

HRangeItemsContainer::HRangeItemsContainer(QWidget *parent): AbstractRangeWidgetsContainer(parent) {
}

void HRangeItemsContainer::updateRangeWidget(RangeBarItem *rw) {
    int x = qRound(rw->begin * vpp);
    int width = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(x, 0, width, rect().height());
}

void HRangeItemsContainer::updateBar() {
    int width = qRound((maxVal - minVal) * vpp);
    setFixedSize(width, parentWidget()->height());
    for (auto* rw : rangeWidgets) {
        updateRangeWidget(rw);
    }
}

RangeBarItem::RangeBarItem(QWidget *parent): Widget(parent), begin(), end() {
    setObjectName("rw");
    setStyleSheet(qss_target("rw", bg(Styles::GRAY_1->rgbHex)));
}

void RangeBarItem::syncDataToWidget() {
    if (!wData) {
        return;
    }
    auto* data = wData->cast<RangeBarData>();
    if (begin != data->begin || end != data->end) {
        begin = data->begin;
        end = data->end;
        emit sigUpdateWidget();
    }
}

void RangeBarItem::syncWidgetToData() {
    if (!wData) {
        return;
    }
    auto* data = wData->cast<RangeBarData>();
    data->begin = begin;
    data->end = end;
}

RangeBar::RangeBar(AbstractRangeWidgetsContainer* c, QWidget *parent): ScrollArea(parent), rootContent() {
    assembled = false;
    rootContent = new QWidget(this);
    rootContent->setObjectName("root");
    rootContent->setStyleSheet(qss_target("root", bg(Styles::CLEAR->rgbHex)));
    setWidget(rootContent);
    if (!c) {
        throwInFunc("null container");
    }
    if (dynamic_cast<VRangeItemsContainer*>(c)) {
        isVertical = true;
    } else if (dynamic_cast<HRangeItemsContainer*>(c)) {
        isVertical = false;
    } else {
        throwInFunc("do not directly override AbstractRangeWidgetsContainer");
    }
    connect(c, &AbstractRangeWidgetsContainer::sigZoom, this, &RangeBar::barDataChanged);
    c->setParent(rootContent);
    container = c;
}

void RangeBar::setData(ListData *d) {
    ScrollArea::setData(d);
}

void RangeBar::initPeriodWidget(RangeBarItem* rw) {
    connect(rw, &RangeBarItem::sigUpdateWidget, [this, rw](){
        container->updateRangeWidget(rw);
    });
    rw->setParent(container);
    container->rangeWidgets << rw;
}

void RangeBar::setBarRange(int minVal, int maxVal) {
    if (minVal >= maxVal) {
        throwInFunc("invalid range");
    }
    container->minVal = minVal;
    container->maxVal = maxVal;
}

void RangeBar::setZoomRange(double minVpp, double maxVpp) {
    if (minVpp >= maxVpp) {
        throwInFunc("invalid range");
    }
    container->minVpp = minVpp;
    container->maxVpp = maxVpp;
}

void RangeBar::setZoomEnabled(bool enabled) {
    container->zoomEnabled = enabled;
}

void RangeBar::setZoomStep(double step) {
    if (step <= 0) {
        throwInFunc("invalid step");
    }
    container->zoomStep = step;
}

void RangeBar::setVpp(double vpp) {
    container->vpp = vpp;
}

void RangeBar::syncDataToWidget() {
    auto* ld = wData->cast<ListData>();
    int j = ld->getChangeEnd();
    int size = container->rangeWidgets.length();
    while (size <= j) {
        initPeriodWidget(createRangeWidget());
        size++;
    }
    for (int i = ld->getChangeBegin(); i <= j; i++) {
        auto* rw = container->rangeWidgets.at(i);
        rw->setData(ld->at(i));
        rw->syncDataToWidget();
    }
}

RangeBarItem *RangeBar::createRangeWidget() {
    return new RangeBarItem();
}

void RangeBar::assembleContainer() {
    auto* l = new QHBoxLayout(rootContent);
    l->setContentsMargins({});
    rootContent->setLayout(l);
    l->addWidget(container);
}

void RangeBar::updateContainerSize() {
    setFixedSize(container->size());
}

void RangeBar::barDataChanged() {
    if (isVertical) {
        auto* bar = verticalScrollBar();
        auto dy = (QCursor::pos() - mapToGlobal(pos())).y();
        double dh = qMax(0, qMin(height(), dy));
        //指针所指位置高度相对于容器高度的比值
        double ratio = ((double)bar->value() / bar->maximum() * (container->height() - viewport()->height()) + dh) / container->height();
        container->updateBar();
        bar->setMaximum(container->height() - viewport()->height());
        ratio = (ratio * container->height() - dh) / (container->height() - viewport()->height());
        bar->setValue(qRound(ratio * bar->maximum()));
    } else {
        auto* bar = horizontalScrollBar();
        auto dx = (QCursor::pos() - mapToGlobal(pos())).x();
        double dw = qMax(0, qMin(width(), dx));
        double ratio = ((double)bar->value() / bar->maximum() * (container->width() - viewport()->width()) + dw) / container->width();
        container->updateBar();
        bar->setMaximum(container->width() - viewport()->width());
        ratio = (ratio * container->width() - dw) / container->width();
        bar->setValue(qRound(ratio * bar->maximum()));
    }
    updateContainerSize();
    emit sigBarDataChanged(container->vpp);
}

void RangeBar::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this](){
        syncDataToWidget();
    });
}

void RangeBar::initBar() {
    assembleContainer();
    barDataChanged();
}

void RangeBar::resizeEvent(QResizeEvent *event) {
    ScrollArea::resizeEvent(event);
    if (!assembled) {
        initBar();
        assembled = true;
    }
}
