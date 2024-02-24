//
// Created by stupid_coder_jyy on 2024/2/23.
//

#include "RangeBar.h"
#include "Error.h"

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
    int y = qRound(rw->beginVal * vpp);
    int height = qRound((rw->endVal - rw->beginVal) * vpp);
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
    int x = qRound(rw->beginVal * vpp);
    int width = qRound((rw->endVal - rw->beginVal) * vpp);
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

RangeWidget::RangeWidget(QWidget *parent):Widget(parent), beginVal(), endVal() {
    setObjectName("rw");
    setStyleSheet(qss_t("rw", bg(Styles::GRAY_1)));
}

void RangeWidget::setRange(int newBegin, int newEnd) {
    beginVal = newBegin;
    endVal = newEnd;
    emit sigRangeChanged(newBegin, newBegin);
}

RangeBar::RangeBar(bool isVertical, QWidget *parent):ScrollArea(parent) {
    if (isVertical) {
        setContainer(new VRangeWidgetsContainer(this));
    } else {
        setContainer(new HRangeWidgetsContainer(this));
    }
}

RangeWidget *RangeBar::createRangeWidget() {
    return new RangeWidget();
}

#define CHECK_CONTAINER if (!container) throwInFunc("null container");

void RangeBar::addPeriod(int begin, int end) {
    CHECK_CONTAINER
    auto* rw = createRangeWidget();
    connect(rw, &RangeWidget::sigRangeChanged, [this, rw](){
        container->updateRangeWidget(rw);
    });
    rw->setParent(container);
    rw->setRange(begin, end);
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

void RangeBar::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    updateBar();
}
