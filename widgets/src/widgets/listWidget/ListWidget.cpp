//
// Created by stupid_coder_jyy on 2024/2/25.
//

#include "ListWidget.h"
#include "ScrollBar.h"
#include "Error.h"
#include "NBT.h"
#include <QDrag>
#include <WidgetFactory.h>

USING_LR

ListItem::ListItem(QWidget *parent, bool initInConstructor): Widget(parent, initInConstructor), listData(), selected(), dataIdx() {
}

void ListItem::syncDataToWidget() {
    if (listData) {
        selected = listData->selectedIdx == dataIdx;
    }
}

void ListItem::mouseReleaseEvent(QMouseEvent *event) {
    if (wData) {
        listData->selectData(dataIdx);
    }
}

ListWidget::ListWidget(QWidget *parent, bool initInConstructor): ScrollArea(parent, initInConstructor),
        container(new QWidget(this)), dragScrollStep(),
        rowHeight(40), areaRowCount(0), pos(0), globalPos(),
        maxGlobalPos(), posMid(), posBottom(), idxA(-1), idxB(-1), factoryItem() {
    setWidget(container);
    prepared = true;
}

void ListWidget::setRowHeight(int s) {
    rowHeight = s;
}

void ListWidget::setMinAreaRowCount(int count) {
    areaRowCount = count;
}

void ListWidget::onPostParsing(Handlers &handlers, NBT *nbt) {
    if (nbt->contains("row_height", Data::INT)) {
        int height = nbt->getInt("row_height");
        handlers << [height](QWidget *target) {
            auto *list = static_cast<ListWidget *>(target);
            list->setRowHeight(height);
        };
    }
    if (nbt->contains("item", Data::COMPOUND)) {
        auto* f = WidgetFactory::fromNbt("item", nbt->get().take("item")->asCompound());
        f->include(WidgetFactory::factoryInParse());
        f->parse();
        if (f->getState() == WidgetFactory::Broken) {
            delete f;
        } else {
            handlers << [f](QWidget *widget) {
                auto *l = static_cast<ListWidget *>(widget);
                l->factoryItem = f;
            };
        }
    }
}

void ListWidget::setData(WidgetData *d) {
    auto* ld = dynamic_cast<ListData*>(d);
    if (d && !ld) {
        throw Error("[ListWidget::setData] requires ListData");
    }
    ScrollArea::setData(d);
}

ListWidget::~ListWidget() {
    delete factoryItem;
}

ListItem *ListWidget::newItem() {
    if (factoryItem) {
        return factoryItem->applyAndCast<ListItem>();
    }
    return new ListItem();
}

void ListWidget::prepareItem(ListItem* w) {
    w->setList(wData->cast<ListData>());
    w->setParent(container);
    w->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    w->setGeometry(0, items.size() * rowHeight, container->width(), rowHeight);
    items << w;
}

void ListWidget::syncItem(ListItem *item, int idx) {
    auto *d = wData->cast<ListData>()->at(idx);
    item->setData(d);
    item->dataIdx = idx;
    item->syncDataToWidget();
}

void ListWidget::setGlobalPos(int gp, bool forceUpdate) {
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

void ListWidget::syncItems(int begin, int end) {
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
        syncItem(items[i], begin);
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

void ListWidget::resizeEvent(QResizeEvent *event) {
    ScrollArea::resizeEvent(event);
    updateBase();
}

void ListWidget::wheelEvent(QWheelEvent *event) {
    int dy = rowHeight;
    if (event->modifiers() == Qt::ControlModifier) {
        dy = dy << 2;
    }
    scroll(event->angleDelta().y() > 0 ? -dy : dy);
}

ScrollBar *ListWidget::createVerticalScrollBar() {
    auto *b = new ScrollBar(this, Qt::Vertical);
    connect(b, &QScrollBar::valueChanged, this, [this](const int v) {
        setGlobalPos(v);
    });
    return b;
}

void ListWidget::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this](){
        auto* d = wData->cast<ListData>();
        syncItems(d->getChangeBegin(), d->getChangeEnd());
    });
}

void ListWidget::updateBase() {
    if (!wData) {
        return;
    }
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
            prepareItem(newItem());
        }
    } else {
        int oldCount = items.length();
        //补上缺失的item
        items.reserve(itemsCount);
        for (int i = oldCount ; i < itemsCount; i++) {
            prepareItem(newItem());
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
        syncItem(items[i], begin);
    }
}

void ListWidget::fillB(int begin, bool forceUpdate) {
    if (!wData || (!forceUpdate && idxB == begin)) {
        return;
    }
    idxB = begin;
    int end = qMin(begin + areaRowCount, wData->cast<ListData>()->length());
    for (int i = areaRowCount; begin < end; i++, begin++) {
        syncItem(items[i], begin);
    }
}

void ListWidget::updateMaxGlobalPos() {
    maxGlobalPos = qMax(0, wData->cast<ListData>()->length() * rowHeight - viewport()->height());
    getVScrollBar()->onRangeChanged(0, maxGlobalPos);
}