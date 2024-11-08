//
// Created by stupid_coder_jyy on 2024/4/2.
//

#include "RangeBar.h"
#include "WidgetUtil.h"
#include "NBT.h"
#include "MemUtil.h"
#include <QHBoxLayout>

BarData::BarData(): NestedListNode(), begin(), end() {
}

BarData::BarData(int begin, int end): begin(begin), end(end) {
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
        QWidget(parent), minVal(0), maxVal(99), vpp(1), maxVpp(1), minVpp(1),
        mainZoomEnabled(false), mainZoomStep(0), sideZoomEnabled(false), sideZoomStep(0),
        minSideLen(40), maxSideLen(40), sideLen(40) {
}

AbstractBarContainer::~AbstractBarContainer() {
    DELETE_LIST(bars)
}

void AbstractBarContainer::wheelEvent(QWheelEvent *event) {
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

VBarContainer::VBarContainer(QWidget *parent): AbstractBarContainer(parent) {
}

void VBarContainer::updateBarGeometry() {
    int height = qRound((maxVal - minVal) * vpp);
    int width = bars.size() * sideLen;
    setFixedSize(width, height);
}

void VBarContainer::updateRangeWidgetGeometry(BarItem *rw) {
    int y = qRound(rw->begin * vpp);
    int height = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(rw->row * sideLen, y, sideLen, height);
}

HBarContainer::HBarContainer(QWidget *parent): AbstractBarContainer(parent) {
}

void HBarContainer::updateBarGeometry() {
    int width = qRound((maxVal - minVal) * vpp);
    int height = bars.size() * sideLen;
    setFixedSize(width, height);
}

void HBarContainer::updateRangeWidgetGeometry(BarItem *rw) {
    int x = qRound(rw->begin * vpp);
    int width = qRound((rw->end - rw->begin) * vpp);
    rw->setGeometry(x, rw->row * sideLen, width, sideLen);
}

void HBarContainer::wheelEvent(QWheelEvent *event) {
    AbstractBarContainer::wheelEvent(event);
    if (!event->isAccepted()) {
        emit sigScroll(event->angleDelta().y() >> 1);
        event->accept();
    }
}

RangeBar::RangeBar(AbstractBarContainer* c, QWidget *parent): ScrollArea(parent) {
    assembled = false;
    rootContent = new QWidget(this);
    rootContent->setObjectName("root");
    rootContent->setStyleSheet(qss_object("root", bg(Styles::CLEAR->rgbHex)));
    setWidget(rootContent);
    if (!c) {
        throwInFunc("null container");
    }
    if (dynamic_cast<VBarContainer*>(c)) {
        isVertical = true;
    } else if (auto* hBar = dynamic_cast<HBarContainer*>(c)) {
        connect(hBar, &HBarContainer::sigScroll, this, [this](int dx){
            auto* bar = horizontalScrollBar();
            bar->setValue(bar->value() - dx);
        });
        isVertical = false;
    } else {
        throwInFunc("do not directly override AbstractRangeWidgetsContainer");
    }
    connect(c, &AbstractBarContainer::sigMainZoom, this, [this](){
        barDataChanged(MainZoom);
    });
    connect(c, &AbstractBarContainer::sigSideZoom, this, [this](){
        barDataChanged(SideZoom);
    });
    container = c;
}

void RangeBar::syncDataToWidget() {
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
            if (!assembled) {
                assemble();
            } else {
                barDataChanged(BarCountChanged);
            }
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

void RangeBar::onPostParsing(StandardWidget::Handlers &handlers, NBT *nbt) {
    if (nbt->contains("barRange", Data::ARR)) {
        int arr[2]{0, 0};
        nbt->get("barRange")->asArray()->fillInt(arr, 2);
        if (arr[0] <= 0 && arr[1] <= 0 && arr[0] >= arr[1]) {
            throwInFunc("invalid barRange");
        }
        handlers << [arr](QWidget* target){
            static_cast<RangeBar*>(target)->setBarRange(arr[0], arr[1]);
        };
    }
    if (nbt->contains("mainZoomRange", Data::ARR)) {
        float arr[2]{0, 0};
        nbt->get("mainZoomRange")->asArray()->fillFloat(arr, 2);
        if (arr[0] <= 0 && arr[1] <= 0 && arr[0] >= arr[1]) {
            throwInFunc("invalid mainZoomRange");
        }
        handlers << [arr](QWidget* target){
            static_cast<RangeBar*>(target)->setMainZoomRange(arr[0], arr[1]);
        };
    }
    if (nbt->contains("sideZoomRange", Data::ARR)) {
        int arr[2]{0, 0};
        nbt->get("sideZoomRange")->asArray()->fillInt(arr, 2);
        if (arr[0] <= 0 && arr[1] <= 0 && arr[0] >= arr[1]) {
            throwInFunc("invalid sideZoomRange");
        }
        handlers << [arr](QWidget* target){
            static_cast<RangeBar*>(target)->setSideZoomRange(arr[0], arr[1]);
        };
    }
    if (nbt->contains("zoomEnabled", Data::ARR)) {
        bool arr[2]{false, false};
        nbt->get("zoomEnabled")->asArray()->fillBool(arr, 2);
        handlers << [arr](QWidget* target){
            static_cast<RangeBar*>(target)->setZoomEnabled(arr[0], arr[1]);
        };
    }
    if (nbt->contains("mainZoomStep", Data::FLOAT)) {
        float s = nbt->getFloat("mainZoomStep");
        if (s <= 0) {
            throwInFunc("invalid mainZoomStep");
        }
        handlers << [s](QWidget* target){
            static_cast<RangeBar*>(target)->setMainZoomStep(s);
        };
    }
    if (nbt->contains("sideZoomStep", Data::INT)) {
        int s = nbt->getInt("sideZoomStep");
        if (s <= 0) {
            throwInFunc("invalid sideZoomStep");
        }
        handlers << [s](QWidget* target){
            static_cast<RangeBar*>(target)->setSideZoomStep(s);
        };
    }
    if (nbt->contains("vpp", Data::FLOAT)) {
        float v = nbt->getFloat("vpp");
        if (v <= 0) {
            throwInFunc("invalid vpp");
        }
        handlers << [v](QWidget* target){
            static_cast<RangeBar*>(target)->setVpp(v);
        };
    }
    if (nbt->contains("sideLen", Data::INT)) {
        int l = nbt->getInt("sideLen");
        if (l <= 0) {
            throwInFunc("invalid sideLen");
        }
        handlers << [l](QWidget* target){
            static_cast<RangeBar*>(target)->setSideLen(l);
        };
    }
}

void RangeBar::setData(WidgetData *d) {
    if (auto* nd = dynamic_cast<NestedListData*>(d)) {
        ScrollArea::setData(nd);
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
        assemble();
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

void RangeBar::updateContentSize() {
    rootContent->setFixedSize(container->size());
}

void RangeBar::initPeriodWidget(BarItem *rw) {
    connect(rw, &BarItem::sigUpdateWidget, [this, rw](){
        container->updateRangeWidgetGeometry(rw);
    });
    rw->setParent(container);
}

void RangeBar::barDataChanged(BarUpdateType type) {
    switch (type) {
        case Init:
            container->updateBarGeometry();
            break;
        case BarCountChanged:
            container->updateBarGeometry();
            updateContentSize();
            emit sigBarLayoutChanged();
            return;
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
    updateContentSize();
    for (auto bar : container->bars) {
        for (auto* item : *bar) {
            container->updateRangeWidgetGeometry(item);
        }
    }
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

void RangeBar::assemble() {
    assembleContainer();
    barDataChanged(Init);
    assembled = true;
}
