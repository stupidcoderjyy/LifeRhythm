//
// Created by stupid_coder_jyy on 2024/2/23.
//

#include "RangeBar.h"
#include "Error.h"

RangeWidgetData::RangeWidgetData(): WidgetData(), begin(), end() {
}

int RangeWidgetData::getBegin() const {
    return begin;
}

int RangeWidgetData::getEnd() const {
    return end;
}

void RangeWidgetData::setBegin(int b) {
    begin = b;
}

void RangeWidgetData::setEnd(int e) {
    end = e;
}

AbstractRangeWidgetsContainer::AbstractRangeWidgetsContainer(QWidget *parent):
        Widget(parent),minVal(0), maxVal(99), vpp(1), maxVpp(10), minVpp(1), zoomEnabled(true), zoomStep(2) {
    setFixedSize(0, 0);
    setObjectName("rc");
    setStyleSheet(qss_t("rc", bg(Styles::BLACK)));
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

VRangeWidgetsContainer::VRangeWidgetsContainer(QWidget *parent):AbstractRangeWidgetsContainer(parent) {
}

void VRangeWidgetsContainer::updateRangeWidget(RangeWidget* rw) {
    int y = qRound(rw->begin * vpp);
    int height = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(0, y, rect().width(), height);
}

void VRangeWidgetsContainer::updateBar() {
    int height = qRound((maxVal - minVal) * vpp);
    int width = rect().width();
    if (width == 0) {
        width = static_cast<QWidget*>(parent())->width();
    }
    setFixedSize(width, height);
    for (auto* rw : rangeWidgets) {
        updateRangeWidget(rw);
    }
}

HRangeWidgetsContainer::HRangeWidgetsContainer(QWidget *parent):AbstractRangeWidgetsContainer(parent) {
}

void HRangeWidgetsContainer::updateRangeWidget(RangeWidget *rw) {
    int x = qRound(rw->begin * vpp);
    int width = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(x, 0, width, rect().height());
}

void HRangeWidgetsContainer::updateBar() {
    int width = qRound((maxVal - minVal) * vpp);
    int height = rect().height();
    if (height == 0) {
        height = static_cast<QWidget*>(parent())->height();
    }
    setFixedSize(width, height);
    for (auto* rw : rangeWidgets) {
        updateRangeWidget(rw);
    }
}

RangeWidget::RangeWidget(QWidget *parent):Widget(parent), begin(), end() {
    setObjectName("rw");
    setStyleSheet(qss_t("rw", bg(Styles::GRAY_1)));
}

void RangeWidget::syncDataToWidget() {
    if (!wData) {
        return;
    }
    auto* data = wData->cast<RangeWidgetData>();
    if (begin != data->begin || end != data->end) {
        begin = data->begin;
        end = data->end;
        emit sigUpdateWidget();
    }
}

void RangeWidget::syncWidgetToData() {
    if (!wData) {
        return;
    }
    auto* data = wData->cast<RangeWidgetData>();
    data->begin = begin;
    data->end = end;
}

RangeBar::RangeBar(bool isVertical, QWidget *parent):ScrollArea(parent) {
    if (isVertical) {
        setContainer(new VRangeWidgetsContainer(this));
    } else {
        setContainer(new HRangeWidgetsContainer(this));
    }
}

void RangeBar::setData(ListData *d) {
    ScrollArea::setData(d);
}

RangeWidget *RangeBar::createRangeWidget() {
    return new RangeWidget();
}

#define CHECK_CONTAINER if (!container) throwInFunc("null container");

void RangeBar::initPeriodWidget(RangeWidget* rw) {
    CHECK_CONTAINER
    connect(rw, &RangeWidget::sigUpdateWidget, [this, rw](){
        container->updateRangeWidget(rw);
    });
    rw->setParent(container);
    container->rangeWidgets << rw;
}


void RangeBar::setContainer(AbstractRangeWidgetsContainer *c) {
    delete container;
    if (!c) {
        throwInFunc("null container");
    }
    if (dynamic_cast<VRangeWidgetsContainer*>(c)) {
        connect(c, &AbstractRangeWidgetsContainer::sigZoom, this, [this](){
            auto* bar = verticalScrollBar();
            double halfVp = (double)height() / 2.0f;
            double ratio = ((double)bar->value() / bar->maximum() * container->height() + halfVp) / container->height();
            container->updateBar();
            ratio = (ratio * container->height() - halfVp) / container->height();
            bar->setValue(qRound(ratio * bar->maximum()));
        });
    } else if (dynamic_cast<HRangeWidgetsContainer*>(c)) {
        connect(c, &AbstractRangeWidgetsContainer::sigZoom, this, [this](){
            auto* bar = horizontalScrollBar();
            double halfVp = (double)width() / 2.0f;
            double ratio = ((double)bar->value() / bar->maximum() * container->width() + halfVp) / container->width();
            container->updateBar();
            ratio = (ratio * container->width() - halfVp) / container->width();
            bar->setValue(qRound(ratio * bar->maximum()));
        });
    } else {
        throwInFunc("do not directly override AbstractRangeWidgetsContainer");
    }
    container = c;
}

void RangeBar::updateBar() {
    CHECK_CONTAINER
    container->updateBar();
    setWidget(container);
}

void RangeBar::setBarRange(int minVal, int maxVal) {
    CHECK_CONTAINER
    if (minVal >= maxVal) {
        throwInFunc("invalid range");
    }
    container->minVal = minVal;
    container->maxVal = maxVal;
}

void RangeBar::setZoomRange(double minVpp, double maxVpp) {
    CHECK_CONTAINER
    if (minVpp >= maxVpp) {
        throwInFunc("invalid range");
    }
    container->minVpp = minVpp;
    container->maxVpp = maxVpp;
}

void RangeBar::setZoomEnabled(bool enabled) {
    CHECK_CONTAINER
    container->zoomEnabled = enabled;
}

void RangeBar::setZoomStep(double step) {
    CHECK_CONTAINER
    if (step <= 0) {
        throwInFunc("invalid step");
    }
    container->zoomStep = step;
}

void RangeBar::setVpp(double vpp) {
    CHECK_CONTAINER
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

void RangeBar::showEvent(QShowEvent *event) {
    ScrollArea::showEvent(event);
    updateBar();
}

QMetaObject::Connection RangeBar::connectModelView() {
    return connect(wData, &WidgetData::sigDataChanged, this, [this](){
        syncDataToWidget();
    });
}