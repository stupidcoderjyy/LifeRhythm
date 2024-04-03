//
// Created by stupid_coder_jyy on 2024/4/2.
//

#include "RangeBarList.h"
#include "WidgetUtil.h"
#include <QHBoxLayout>

AbstractBarListContainer::AbstractBarListContainer(QWidget *parent): QWidget(parent),
        minVal(0), maxVal(99), vpp(1), maxVpp(1), minVpp(1), mainZoomEnabled(true),
        mainZoomStep(0), sideZoomEnabled(true), sideZoomStep(0),
        minSideLen(40), maxSideLen(40), sideLen(40) {
}

void AbstractBarListContainer::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() == Qt::ControlModifier) {
        if (!mainZoomEnabled) {
            event->accept();
            return;
        }
        if (event->angleDelta().y() > 0) {
            if (vpp != maxVpp) {
                vpp = qMin(maxVpp, vpp + mainZoomStep);
                emit sigMainZoom();
            }
        } else if (vpp != minVpp) {
            vpp = qMax(minVpp, vpp - mainZoomStep);
            emit sigMainZoom();
        }
        event->accept();
    } else if (event->modifiers() == Qt::ShiftModifier) {
        if (!sideZoomEnabled) {
            event->accept();
            return;
        }
        if (event->angleDelta().y() > 0) {
            if (sideLen != maxSideLen) {
                sideLen = qMin(maxSideLen, sideLen + sideZoomStep);
                emit sigSideZoom();
            }
        } else if (sideLen != minSideLen) {
            sideLen = qMax(minSideLen, sideLen - sideZoomStep);
            emit sigSideZoom();
        }
        event->accept();
    }
}

VBarListContainer::VBarListContainer(QWidget *parent): AbstractBarListContainer(parent) {
}

void VBarListContainer::updateBarGeometry() {
    int height = qRound((maxVal - minVal) * vpp);
    int width = bars.size() * sideLen;
    setFixedSize(width, height);
}

void VBarListContainer::updateRangeWidgetGeometry(BarItem *rw) {
    int y = qRound(rw->begin * vpp);
    int height = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(rw->row * sideLen, y, sideLen, height);
}

HBarListContainer::HBarListContainer(QWidget *parent) : AbstractBarListContainer(parent) {
}

void HBarListContainer::updateBarGeometry() {
    int width = qRound((maxVal - minVal) * vpp);
    int height = bars.size() * sideLen;
    setFixedSize(width, height);
}

void HBarListContainer::updateRangeWidgetGeometry(BarItem *rw) {
    int x = qRound(rw->begin * vpp);
    int width = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(x, rw->row * sideLen, width, sideLen);
}

void HBarListContainer::wheelEvent(QWheelEvent *event) {
    AbstractBarListContainer::wheelEvent(event);
    if (!event->isAccepted()) {
        emit sigScroll(event->angleDelta().y() >> 1);
        event->accept();
    }
}

RangeBarList::RangeBarList(AbstractBarListContainer* c, QWidget *parent): ScrollArea(parent) {
    assembled = false;
    rootContent = new QWidget(this);
    rootContent->setObjectName("root");
    rootContent->setStyleSheet(qss_target("root", bg(Styles::CLEAR->rgbHex)));
    setWidget(rootContent);
    if (!c) {
        throwInFunc("null container");
    }
    if (dynamic_cast<VBarListContainer*>(c)) {
        isVertical = true;
    } else if (auto* hBar = dynamic_cast<HBarListContainer*>(c)) {
        connect(hBar, &HBarListContainer::sigScroll, this, [this](int dx){
            auto* bar = horizontalScrollBar();
            bar->setValue(bar->value() - dx);
        });
        isVertical = false;
    } else {
        throwInFunc("do not directly override AbstractRangeWidgetsContainer");
    }
    connect(c, &AbstractBarListContainer::sigMainZoom, this, [this](){
        barDataChanged(MainZoom);
    });
    connect(c, &AbstractBarListContainer::sigSideZoom, this, [this](){
        barDataChanged(SideZoom);
    });
    container = c;
}

void RangeBarList::syncDataToWidget() {
    auto* nd = wData->cast<NestedListData>();
    switch (nd->getEditType()) {
        case NestedListData::None:
            return;
        case NestedListData::Row: {
            int j = nd->getRowChangeEnd();
            int size = container->bars.length();
            while (size++ <= j) {
                container->bars << new QVector<BarItem*>();
            }
            barDataChanged(BarCountChanged);
            break;
        }
        case NestedListData::Column: {
            auto* bar = container->bars[nd->getDirtyRow()];
            int j = nd->getColumnChangeEnd();
            int size = bar->length();
            while (size++ <= j) {
                auto* rw = createRangeWidget();
                initPeriodWidget(rw);
                bar->append(rw);
            }
            for (int i = nd->getColumnChangeBegin(); i <= j; i++) {
                auto* rw = bar->at(i);
                rw->setData(nd->at(nd->getDirtyRow(), i));
                rw->syncDataToWidget();
            }
            break;
        }
    }
}

void RangeBarList::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this](){
        syncDataToWidget();
    });
}

void RangeBarList::resizeEvent(QResizeEvent *event) {
    ScrollArea::resizeEvent(event);
    if (!assembled) {
        assembleContainer();
        barDataChanged(Init);
        assembled = true;
    }
}

BarItem *RangeBarList::createRangeWidget() {
    return new BarItem();
}

void RangeBarList::assembleContainer() {
    auto* l = new QHBoxLayout(rootContent);
    l->setContentsMargins({});
    rootContent->setLayout(l);
    l->addWidget(container);
}

void RangeBarList::updateContainerSize() {
    rootContent->setFixedSize(container->size());
}

void RangeBarList::initPeriodWidget(BarItem *rw) {
    connect(rw, &BarItem::sigUpdateWidget, [this, rw](){
        container->updateRangeWidgetGeometry(rw);
    });
    rw->setParent(container);
}

void RangeBarList::barDataChanged(BarUpdateType type) {
    switch (type) {
        case Init:
            container->updateBarGeometry();
            for (auto bar : container->bars) {
                for (auto* item : *bar) {
                    container->updateRangeWidgetGeometry(item);
                }
            }
            break;
        case BarCountChanged:
            container->updateBarGeometry();
            break;
        case MainZoom:
            if (isVertical) {
                performVerticalZoom();
            } else {
                performHorizontalZoom();
            }
            break;
        case SideZoom:
            if (isVertical) {
                performHorizontalZoom();
            } else {
                performVerticalZoom();
            }
            break;
    }
    updateContainerSize();
    emit sigBarLayoutChanged();
}

void RangeBarList::performHorizontalZoom() {
    auto* bar = horizontalScrollBar();
    auto dx = (QCursor::pos() - getGlobalPos(this)).x();
    double dw = qMax(0, qMin(width(), dx));
    double ratio = ((double)bar->value() / bar->maximum() * (container->width() - viewport()->width()) + dw) / container->width();
    container->updateBarGeometry();
    bar->setMaximum(container->width() - viewport()->width());
    bar->setValue(qRound(ratio * container->width() - dw));
}

void RangeBarList::performVerticalZoom() {
    auto* bar = verticalScrollBar();
    auto dy = (QCursor::pos() - getGlobalPos(this)).y();
    double dh = qMax(0, qMin(height(), dy));
    double ratio = ((double)bar->value() / bar->maximum() * (container->height() - viewport()->height()) + dh) / container->height();
    container->updateBarGeometry();
    bar->setMaximum(container->height() - viewport()->height());
    bar->setValue(qRound(ratio * container->height() - dh));
}