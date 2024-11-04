//
// Created by JYY on 24-11-2.
//

#include "TiledListWidget.h"
#include "ListWidget.h"
#include "ScrollBar.h"
#include "Error.h"
#include "NBT.h"
#include <QDrag>

TiledListWidget::TiledListWidget(QWidget *parent): ScrollArea(parent), container(new QWidget(this)),
        dragScrollStep(), rowHeight(40), areaRowCount(0), pos(0), globalPos(), maxGlobalPos(),
        posMid(), posBottom(), idxA(-1), idxB(-1), columnCount(1), columnWidth(50), columnPolicy(Auto) {
    setWidget(container);
    scrollTimer.setInterval(5);
    connect(&scrollTimer, &QTimer::timeout, this, [this] {
        setGlobalPos(globalPos + dragScrollStep, false);
    });
}

void TiledListWidget::setRowHeight(int s) {
    rowHeight = s;
}

void TiledListWidget::setMinAreaRowCount(int count) {
    areaRowCount = count;
}

void TiledListWidget::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    if (!widgetTag->contains("row_height", Data::INT)) {
        return;
    }
    int height = widgetTag->getInt("row_height");
    handlers << [height](QWidget* target) {
        auto* list = static_cast<TiledListWidget*>(target);
        list->setRowHeight(height);
    };
}

void TiledListWidget::setData(WidgetData *d) {
    if (dynamic_cast<ListData*>(d)) {
        ScrollArea::setData(d);
    }
}

void TiledListWidget::setColumnPolicy(ColumnPolicy policy) {
    columnPolicy = policy;
}

void TiledListWidget::setColumnWidth(int width) {
    if (width <= 0) {
        throwInFunc("Width must be greater than zero: " + QString::number(width));
    }
    columnWidth = width;
}

void TiledListWidget::setColumnCount(int count) {
    if (count < 0) {
        throwInFunc("Count must be greater than zero: " + QString::number(count));
    }
    columnCount = count;
}

void TiledListWidget::resizeEvent(QResizeEvent *event) {
    ScrollArea::resizeEvent(event);
    updateListBase();
}

void TiledListWidget::wheelEvent(QWheelEvent *event) {
    int dy = rowHeight;
    if (event->modifiers() == Qt::ControlModifier) {
        dy = dy << 2;
    }
    scroll(event->angleDelta().y() > 0 ? -dy : dy);
}

ScrollBar* TiledListWidget::createVerticalScrollBar() {
    auto* b = new ScrollBar(this, Qt::Vertical);
    connect(b, &QScrollBar::valueChanged, this, [this](const int v){
        setGlobalPos(v, false);
    });
    return b;
}

ListItem *TiledListWidget::createRowItem() {
    return new ListItem();
}

void TiledListWidget::prepareNewItem(ListItem* w) {
    w->setList(wData->cast<ListData>());
    w->setParent(container);
    w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(w, &ListItem::sigDragEnter, this, &TiledListWidget::onItemDragEnter);
    connect(w, &ListItem::sigDragLeave, this, &TiledListWidget::onItemDragLeave);
    connect(w, &ListItem::sigDragStart, this, &TiledListWidget::onItemDragStart);
    connect(w, &ListItem::sigDragMove, this, [this](ListItem* item, QDragMoveEvent* evt){
        onItemDragMove(item, evt);
        performDragScroll(item, evt);
    });
    connect(w, &ListItem::sigDropped, this, [this, w](ListItem* src){
        onItemDropped(src, w);
    });
    connect(w, &ListItem::sigDragEnd, this, [this] {
        scrollTimer.stop();
    });
    w->show();
    items << w;
}

void TiledListWidget::onItemDragStart(ListItem *item) {
}

void TiledListWidget::onItemDragEnter(ListItem *item) {
}

void TiledListWidget::onItemDragLeave(ListItem *item) {
}

void TiledListWidget::onItemDragMove(ListItem* item, QDragMoveEvent *event) {
}

void TiledListWidget::onItemDropped(ListItem *src, ListItem *dest) {
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

void TiledListWidget::onDataChanged(int begin, int end) {
    int rBorder = pos <= posMid && idxB != idxA + areaRowCount ?
                  idxA + areaRowCount :
                  idxA + (areaRowCount << 1);
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
        setItemData(items[i++], begin++);
    }
    updateMaxGlobalPos();   //对于removeLast的情况，进度条会自动调整
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
    if (dy > 0) {   //下行
        if (pos > posBottom) {
            pos -= areaHeight;
            fillA(idxB);
            fillB(idxA + areaRowCount);
        } else if (oldPos <= posMid && pos > posMid) {
            fillB(idxA + areaRowCount);
        }
    } else {    //上行
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

void TiledListWidget::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this] {
        auto* d = wData->cast<ListData>();
        onDataChanged(d->getChangeBegin() / columnCount, getMaxRowCount(d->getChangeEnd()));
    });
}

void TiledListWidget::setItemData(ListItem *item, int idx) const {
    auto* d = wData->cast<ListData>()->at(idx);
    item->setData(d);
    item->dataIdx = idx;
    item->syncDataToWidget();
}

void TiledListWidget::updateListBase() {
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
        prepareNewItem(createRowItem());
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
        setItemData(items[i++], begin++);
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
        setItemData(items[i++], begin++);
    }
}

void TiledListWidget::updateMaxGlobalPos() {
    maxGlobalPos = qMax(0, getMaxRowCount(wData->cast<ListData>()->length()) * rowHeight - viewport()->height());
    getVScrollBar()->onRangeChanged(0, maxGlobalPos);
}

void TiledListWidget::performDragScroll(const ListItem* src, const QDragMoveEvent *event) {
    if (int y = (event->pos() + src->pos()).y() - pos; y < 30) {
        dragScrollStep = -2;
        scrollTimer.start();
    } else if (y > viewport()->height() - 30) {
        dragScrollStep = 2;
        scrollTimer.start();
    } else {
        scrollTimer.stop();
    }
}