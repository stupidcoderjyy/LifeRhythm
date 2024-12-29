//
// Created by JYY on 24-11-2.
//

#include "TiledListWidget.h"
#include "ListWidget.h"
#include "ScrollBar.h"
#include "Error.h"
#include "NBT.h"
#include <QDrag>

USING_LR

TiledListWidget::TiledListWidget(QWidget *parent, bool i): ScrollArea(parent, i), container(new QWidget(this)),
       dragScrollStep(), rowHeight(40), areaRowCount(0), pos(0), globalPos(), maxGlobalPos(),
       posMid(), posBottom(), idxA(-1), idxB(-1), columnCount(1), columnWidth(50), columnPolicy(Auto) {
    setWidget(container);
}

void TiledListWidget::setSlotSize(int w, int h) {
    if (w <= 0 || h <= 0) {
        throw Error("TiledListWidget::setSlotSize(): invalid size:" + QString::number(w) + "x" + QString::number(h));
    }
    rowHeight = h;
    columnWidth = w;
    updateBase();
}

void TiledListWidget::setColumnCount(int columns) {
    if (columns <= 0) {
        throw Error("TiledListWidget::setColumnCount(): invalid args:" + QString::number(columns));
    }
    columnCount = columns;
    updateBase();
}

void TiledListWidget::setData(WidgetData *d) {
    if (!dynamic_cast<ListData*>(d)) {
        throw Error("TiledListWidget::setData(): requires ListData");
    }
    ScrollArea::setData(d);
    updateBase();
}

void TiledListWidget::setColumnPolicy(ColumnPolicy policy) {
    columnPolicy = policy;
    updateBase();
}

ListItem *TiledListWidget::newItem() {
    return new ListItem();
}

void TiledListWidget::prepareItem(ListItem* w) {
    w->setList(wData->cast<ListData>());
    w->setParent(container);
    w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    w->show();
    items << w;
}

void TiledListWidget::setGlobalPos(int gp, bool forceUpdate) {
    if (!forceUpdate && globalPos == gp) {
        return;
    }
    gp = qMin(maxGlobalPos, qMax(0, gp));
    int areaHeight = container->height() >> 1;
    if (!forceUpdate) {
        if (int resPos = pos + gp - globalPos; resPos >= 0 && resPos < areaHeight) {
            scroll(gp - globalPos);
            return;
        }
    }
    pos = gp % areaHeight;
    int i = gp / rowHeight;
    i -= i % areaRowCount;
    fillA(i, forceUpdate);
    if (pos > posMid) {
        fillB(i + areaRowCount, forceUpdate);
    }
    globalPos = gp;
    verticalScrollBar()->setValue(pos);
    getVScrollBar()->setValue(globalPos);
}

void TiledListWidget::scroll(int dy) {
    int oldPos = pos;
    globalPos += dy;
    pos += dy;
    if (globalPos < 0) {
        globalPos = 0;
        pos = 0;
    } else if (globalPos > maxGlobalPos) {
        pos -= globalPos - maxGlobalPos;
        globalPos = maxGlobalPos;
    }
    if (pos == oldPos) {
        return;
    }
    int areaHeight = container->height() >> 1;
    if (dy > 0) {
        //下行
        if (pos > posBottom) {
            pos -= areaHeight;
            fillA(idxB);
            fillB(idxA + areaRowCount);
        } else if (oldPos <= posMid && pos > posMid) {
            fillB(idxA + areaRowCount);
        }
    } else {
        //上行
        if (pos < 0) {
            pos += areaHeight;
            fillB(idxA);
            fillA(idxB - areaRowCount);
        } else if (oldPos >= areaHeight && pos < areaHeight) {
            fillA(idxB - areaRowCount);
        }
    }
    verticalScrollBar()->setValue(pos);
    getVScrollBar()->setValue(globalPos);
}

void TiledListWidget::syncItems(int begin, int end) {
    int rBorder = pos <= posMid && idxB != idxA + areaRowCount ? idxA + areaRowCount : idxA + (areaRowCount << 1);
    if (end < idxA || begin >= rBorder) {
        return;
    }
    begin = qMax(begin, idxA);
    int i = begin - idxA;
    int j = qMin(end, rBorder - 1) - idxA;
    i *= columnCount;
    j = (j + 1) * columnCount;
    begin *= columnCount;
    while (i < j) {
        syncItem(items[i++], begin++);
    }
    updateMaxGlobalPos(); //对于removeLast的情况，进度条会自动调整
}

void TiledListWidget::syncItem(ListItem* item, int dataIdx) const {
    auto d = wData->cast<ListData>()->at(dataIdx);
    item->setData(d);
    item->dataIdx = dataIdx;
    item->syncDataToWidget();
}

void TiledListWidget::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this] {
        auto* d = wData->cast<ListData>();
        syncItems(d->getChangeBegin() / columnCount, getMaxRowCount(d->getChangeEnd()));
    });
}

void TiledListWidget::resizeEvent(QResizeEvent *event) {
    ScrollArea::resizeEvent(event);
    updateBase();
}

void TiledListWidget::wheelEvent(QWheelEvent *event) {
    int dy = rowHeight;
    if (event->modifiers() == Qt::ControlModifier) {
        dy = dy << 2;
    }
    scroll(event->angleDelta().y() > 0 ? -dy : dy);
}

ScrollBar *TiledListWidget::createVerticalScrollBar() {
    auto *b = new ScrollBar(this, Qt::Vertical);
    connect(b, &QScrollBar::valueChanged, this, [this](const int v) {
        setGlobalPos(v, false);
    });
    return b;
}

void TiledListWidget::updateBase() {
    if (!wData) {
        return;
    }
    int oldColumn = columnCount;
    int oldAreaCount = areaRowCount;
    if (columnPolicy == Auto) {
        columnCount = width() / columnWidth;
    }
    updateMaxGlobalPos();
    areaRowCount = viewport()->height() / rowHeight + 2;
    if (columnCount == oldColumn && areaRowCount == oldAreaCount) {
        return;
    }
    const int vw = viewport()->width(), vh = viewport()->height();
    const int ah = areaRowCount * rowHeight, aw = qMax(vw, columnCount * columnWidth);
    container->setFixedSize(aw, ah << 1);
    verticalScrollBar()->setRange(0, (ah << 1) - vh);
    horizontalScrollBar()->setRange(0, aw - vw);
    posMid = ah - vh;
    posBottom = posMid + ah;
    int totalRowCounts = areaRowCount << 1;
    int totalItems = totalRowCounts * columnCount;
    items.reserve(totalItems);
    int oldItems = items.length();
    for (int i = oldItems ; i < totalItems ; i ++) {
        prepareItem(newItem());
    }
    int i = 0, x = 0, y = 0;
    for (int r = 0 ; r < totalRowCounts ; r++, x = 0, y += rowHeight) {
        for (int c = 0; c < columnCount ; c++) {
            auto item = items[i++];
            item->setGeometry(x, y, columnWidth, rowHeight);
            item->setVisible(true);
            x += columnWidth;
        }
    }
    while (i < items.length()) {
        auto item = items[i++];
        item->setVisible(false);
    }
    idxA = idxB = -1;
    setGlobalPos(0, true);
}

void TiledListWidget::fillA(int beginRow, bool forceUpdate) {
    if (!wData || (!forceUpdate && idxA == beginRow)) {
        return;
    }
    idxA = beginRow;
    int begin = beginRow * columnCount;
    int end = qMin((beginRow + areaRowCount) * columnCount, wData->cast<ListData>()->length());
    int i = 0;
    while (begin < end) {
        syncItem(items[i++], begin++);
    }
}

void TiledListWidget::fillB(int beginRow, bool forceUpdate) {
    if (!wData || (!forceUpdate && idxB == beginRow)) {
        return;
    }
    idxB = beginRow;
    int begin = beginRow * columnCount;
    int end = qMin((beginRow + areaRowCount) * columnCount, wData->cast<ListData>()->length());
    int i = areaRowCount * columnCount;
    while (begin < end) {
        syncItem(items[i++], begin++);
    }
}

void TiledListWidget::updateMaxGlobalPos() {
    maxGlobalPos = qMax(0, getMaxRowCount(wData->cast<ListData>()->length()) * rowHeight - viewport()->height());
    getVScrollBar()->onRangeChanged(0, maxGlobalPos);
}