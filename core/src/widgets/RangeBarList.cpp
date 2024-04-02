//
// Created by stupid_coder_jyy on 2024/4/2.
//

#include "RangeBarList.h"
#include "Error.h"

AbstractBarListContainer::AbstractBarListContainer(QWidget *parent): QWidget(parent),
        minVal(0), maxVal(99), vpp(1), maxVpp(10), minVpp(1), mainZoomEnabled(true),
        mainZoomStep(2), sideZoomEnabled(true), sideZoomStep(1),
        minSideLen(30), maxSideLen(50), sideLen(40) {
}

void AbstractBarListContainer::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() == Qt::ControlModifier || mainZoomEnabled) {
        if (event->angleDelta().y() > 0) {
            if (vpp != maxVpp) {
                vpp = qMin(maxVpp, vpp + mainZoomStep);
                emit sigMainZoom();
            }
        } else if (vpp != minVpp) {
            vpp = qMax(minVpp, vpp - mainZoomStep);
            emit sigMainZoom();
        }
    }
    if (event->modifiers() == Qt::ShiftModifier || sideZoomEnabled) {
        if (event->angleDelta().y() > 0) {
            if (sideLen != maxSideLen) {
                sideLen = qMin(maxSideLen, sideLen + sideZoomStep);
                emit sigSideZoom();
            }
        } else if (sideLen != minSideLen) {
            sideLen = qMax(minSideLen, sideLen - sideZoomStep);
            emit sigSideZoom();
        }
    }
    event->accept();
}

VBarListContainer::VBarListContainer(QWidget *parent): AbstractBarListContainer(parent) {
}

void VBarListContainer::updateBarGeometry() {
    int height = qRound((maxVal - minVal) * vpp);
    int width = bars.size() * sideLen;
    setFixedSize(width, height);
    for (int i = 0; i < bars.size(); i++) {
        for (auto* item : bars.at(i)) {
            updateRangeWidgetGeometry(i, item);
        }
    }
}

void VBarListContainer::updateRangeWidgetGeometry(int bar, RangeBarItem *rw) {
    int y = qRound(rw->begin * vpp);
    int height = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(bar * sideLen, y, sideLen, height);
}

HBarListContainer::HBarListContainer(QWidget *parent) : AbstractBarListContainer(parent) {
}

void HBarListContainer::updateBarGeometry() {
    int width = qRound((maxVal - minVal) * vpp);
    int height = bars.size() * sideLen;
    setFixedSize(width, height);
    for (int i = 0; i < bars.size(); i++) {
        for (auto* item : bars.at(i)) {
            updateRangeWidgetGeometry(i, item);
        }
    }
}

void HBarListContainer::updateRangeWidgetGeometry(int bar, RangeBarItem *rw) {
    int x = qRound(rw->begin * vpp);
    int width = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(x, bar * sideLen, width, sideLen);
}

RangeBarList::RangeBarList(AbstractBarListContainer* c, QWidget *parent):
    ScrollArea(parent) {
    container = c;
}

void RangeBarList::setBarRange(int minVal, int maxVal) {
    if (minVal >= maxVal) {
        throwInFunc("invalid bar range");
    }
    container->minVal = minVal;
    container->maxVal = maxVal;
}

void RangeBarList::setZoomEnabled(bool main, bool side) {
    container->mainZoomEnabled = main;
    container->sideZoomEnabled = side;
}

void RangeBarList::setMainZoomRange(double minVpp, double maxVpp) {
    if (minVpp >= maxVpp) {
        throwInFunc("invalid main zoom range");
    }
    container->minVpp = minVpp;
    container->maxVpp = maxVpp;
}

void RangeBarList::setMainZoomStep(double step) {
    container->mainZoomStep = step;
}

void RangeBarList::setVpp(double vpp) {
    container->vpp = vpp;
}

void RangeBarList::setSideZoomStep(int step) {
    container->sideZoomStep = step;
}

void RangeBarList::setSideZoomRange(int minSideLen, int maxSideLen) {
    if (minSideLen >= maxSideLen) {
        throwInFunc("invalid side zoom range");
    }
    container->minSideLen = minSideLen;
    container->maxSideLen = maxSideLen;
}
