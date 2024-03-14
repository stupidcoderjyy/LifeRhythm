//
// Created by stupid_coder_jyy on 2024/2/25.
//

#include "ListWidget.h"
#include "ScrollBar.h"
#include "Error.h"
#include "NBT.h"
#include <QDrag>
#include <QMimeData>

ListItem::ListItem(QWidget *parent): Widget(parent), dataIdx(), dragStart() {
}

void ListItem::syncDataToWidget() {
}

void ListItem::syncWidgetToData() {
}

void ListItem::dragEnterEvent(QDragEnterEvent *event) {
    if (event->source() == this) {
        event->ignore();
    } else {
        event->accept();
    }
    emit sigDragEnter(this);
}

void ListItem::dragMoveEvent(QDragMoveEvent *event) {
    emit sigDragMove(this, event);
}

void ListItem::dragLeaveEvent(QDragLeaveEvent *event) {
    emit sigDragLeave(this);
}

void ListItem::dropEvent(QDropEvent *event) {
    emit sigDropped(this);
}

void ListItem::mouseMoveEvent(QMouseEvent *event) {
    if (!wData) {
        return;
    }
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    if (acceptDrops() && (event->pos() - dragStart).manhattanLength() >= (height() >> 1)) {
        auto* drag = new QDrag(this);
        auto* md = new QMimeData();
        drag->setMimeData(md);
        emit sigDragStart(this);
        drag->exec();
        emit sigDragEnd(this);
    }
}

void ListItem::mousePressEvent(QMouseEvent *event) {
    if (!wData) {
        return;
    }
    if (acceptDrops() && event->buttons() & Qt::LeftButton) {
        dragStart = event->pos();
    }
}

ListWidget::ListWidget(QWidget *parent): ScrollArea(parent), rowHeight(40),
        areaRowCount(0), container(new QWidget(this)), pos(0), items(),
        idxA(-1), idxB(-1), globalPos(), scrollTimer(), dragScrollStep() {
    setWidget(container);
    layout = new QVBoxLayout(container);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    container->setLayout(layout);
    scrollTimer.setInterval(5);
    connect(&scrollTimer, &QTimer::timeout, this, [this](){
        setGlobalPos(globalPos + dragScrollStep);
    });
}

void ListWidget::setRowHeight(int s) {
    rowHeight = s;
}

void ListWidget::setMinAreaRowCount(int count) {
    areaRowCount = count;
}

ListItem *ListWidget::createRowItem() {
    return new ListItem();
}

void ListWidget::prepareNewItem(ListItem* w) {
    w->setParent(container);
    w->setFixedHeight(rowHeight);
    w->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    connect(w, &ListItem::sigDragEnter, this, &ListWidget::onItemDragEnter);
    connect(w, &ListItem::sigDragLeave, this, &ListWidget::onItemDragLeave);
    connect(w, &ListItem::sigDragStart, this, &ListWidget::onItemDragStart);
    connect(w, &ListItem::sigDragMove, this, [this](ListItem* item, QDragMoveEvent* evt){
        onItemDragMove(item, evt);
        performDragScroll(item, evt);
    });
    connect(w, &ListItem::sigDropped, this, [this, w](ListItem* src){
        onItemDropped(src, w);
    });
    connect(w, &ListItem::sigDragEnd, this, [this](){
        scrollTimer.stop();
    });
    layout->addWidget(w);
    items << w;
}

void ListWidget::wheelEvent(QWheelEvent *event) {
    int dy = rowHeight;
    if (event->modifiers() == Qt::ControlModifier) {
        dy = dy << 2;
    }
    scroll(event->angleDelta().y() > 0 ? -dy : dy);
}

void ListWidget::onItemDragStart(ListItem *item) {
}

void ListWidget::onItemDragEnter(ListItem *item) {
}

void ListWidget::onItemDragLeave(ListItem *item) {
}

void ListWidget::onItemDragMove(ListItem* item, QDragMoveEvent *event) {
}

void ListWidget::onItemDropped(ListItem *src, ListItem *dest) {
}

void ListWidget::setGlobalPos(int gp, bool forceUpdate) {
    if (!forceUpdate && globalPos == gp) {
        return;
    }
    gp = qMin(maxGlobalPos, qMax(0, gp));
    int areaHeight = container->height() >> 1;
    if (!forceUpdate) {
        int resPos = pos + gp - globalPos;
        if (resPos >= 0 && resPos < areaHeight) {
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

void ListWidget::onDataChanged(int begin, int end) {
    int rBorder = pos <= posMid && idxB != idxA + areaRowCount ?
            idxA + areaRowCount :
            idxA + (areaRowCount << 1);
    if (end < idxA || begin >= rBorder) {
        return;
    }
    begin = qMax(begin, idxA);
    int i = begin - idxA;
    int j = qMin(end, rBorder - 1) - idxA;
    while (i <= j) {
        setItemData(items[i], begin);
        i++;
        begin++;
    }
    updateMaxGlobalPos();   //对于removeLast的情况，进度条会自动调整
}

void ListWidget::scroll(int dy) {
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

void ListWidget::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this](){
        auto* d = wData->cast<ListData>();
        onDataChanged(d->getChangeBegin(), d->getChangeEnd());
    });
}

void ListWidget::onPostParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    if (!widgetTag->contains("row_height", Data::INT)) {
        return;
    }
    int height = widgetTag->getInt("row_height");
    handlers << [height](QWidget* target) {
        auto* list = static_cast<ListWidget*>(target);
        list->setRowHeight(height);
    };
}

void ListWidget::setData(ListData *d) {
    ScrollArea::setData(d);
}

void ListWidget::resizeEvent(QResizeEvent *event) {
    ScrollArea::resizeEvent(event);
    updateListBase();
}

ScrollBar* ListWidget::createVerticalScrollBar() {
    auto* b = new ScrollBar(this, Qt::Vertical);
    connect(b, &QScrollBar::valueChanged, this, [this](int v){
        setGlobalPos(v);
    });
    return b;
}

void ListWidget::updateListBase() {
    updateMaxGlobalPos();
    int newRowCount = viewport()->height() / rowHeight + 2;
    if (areaRowCount >= newRowCount) {
        return;
    }
    areaRowCount = newRowCount;
    int areaHeight = areaRowCount * rowHeight;
    container->setFixedHeight(areaHeight << 1);
    verticalScrollBar()->setRange(0, (areaHeight << 1) - viewport()->height());
    posMid = areaHeight - viewport()->height();
    posBottom = posMid + areaHeight;
    int itemsCount = areaRowCount << 1;
    if (items.empty()) {
        items.reserve(itemsCount);
        for (int i = 0; i < itemsCount; i++) {
            prepareNewItem(createRowItem());
        }
    } else {
        int oldCount = items.length();
        //补上缺失的item
        items.reserve(itemsCount);
        for (int i = oldCount ; i < itemsCount; i++) {
            prepareNewItem(createRowItem());
        }
    }
    setGlobalPos(globalPos, true);
}

void ListWidget::fillA(int begin, bool forceUpdate) {
    if (!wData || (!forceUpdate && idxA == begin)) {
        return;
    }
    idxA = begin;
    int end = qMin(begin + areaRowCount, wData->cast<ListData>()->length());
    for (int i = 0; begin < end; i++, begin++) {
        setItemData(items[i], begin);
    }
}

void ListWidget::fillB(int begin, bool forceUpdate) {
    if (!wData || (!forceUpdate && idxB == begin)) {
        return;
    }
    idxB = begin;
    int end = qMin(begin + areaRowCount, wData->cast<ListData>()->length());
    for (int i = areaRowCount; begin < end; i++, begin++) {
        setItemData(items[i], begin);
    }
}

void ListWidget::setItemData(ListItem *item, int idx) {
    auto* d = wData->cast<ListData>()->at(idx);
    item->setData(d);
    item->dataIdx = idx;
    item->syncDataToWidget();
}

void ListWidget::updateMaxGlobalPos() {
    maxGlobalPos = qMax(0, wData->cast<ListData>()->length() * rowHeight - viewport()->height());
    getVScrollBar()->onRangeChanged(0, maxGlobalPos);
}

void ListWidget::performDragScroll(ListItem* src, QDragMoveEvent *event) {
    int y = (event->pos() + src->pos()).y() - pos;
    if (y < 30) {
        dragScrollStep = -2;
        scrollTimer.start();
    } else if (y > viewport()->height() - 30) {
        dragScrollStep = 2;
        scrollTimer.start();
    } else {
        scrollTimer.stop();
    }
}