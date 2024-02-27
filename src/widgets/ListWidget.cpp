//
// Created by stupid_coder_jyy on 2024/2/25.
//

#include "ListWidget.h"
#include "Styles.h"
#include "ScrollBar.h"
#include "Error.h"
#include "NBT.h"
#include <QDrag>
#include <QMimeData>

ListItem::ListItem(QWidget *parent): Widget(parent),
        data(), dc(), dataIdx(), dragStart() {
}

void ListItem::setData(WidgetData *d) {
    if (data == d) {
        return;
    }
    if (data) {
        disconnect(dc);
    }
    if (d) {
        dc = connect(d, &WidgetData::sigDataChanged, this, &ListItem::syncDataToWidget);
    }
    data = d;
}

void ListItem::syncDataToWidget() {
}

void ListItem::syncWidgetToData() {
}

void ListItem::clearWidget() {
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
    if (acceptDrops() && event->buttons() & Qt::LeftButton) {
        dragStart = event->pos();
    }
}

ListWidget::ListWidget(QWidget *parent): QScrollArea(parent), StandardWidget(), rowHeight(40),
        areaRowCount(), container(new QWidget(this)), pos(), items(), model(), posBottom(),
        posMid(), idxA(-1), idxB(-1), globalPos(), maxGlobalPos(), scrollTimer(), dragScrollStep() {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vBar = new ScrollBar(this, Qt::Vertical);
    hBar = new ScrollBar(this, Qt::Horizontal);
    connect(horizontalScrollBar(), &QScrollBar::valueChanged, hBar, &ScrollBar::onValueSet);
    connect(hBar, &QScrollBar::valueChanged, horizontalScrollBar(), &QScrollBar::setValue);
    connect(horizontalScrollBar(), &QScrollBar::rangeChanged, hBar, &ScrollBar::onRangeChanged);
    connect(vBar, &QScrollBar::valueChanged, this, [this](int v){
        setGlobalPos(v);
    });
    setFrameShape(QFrame::NoFrame);
    setWidgetResizable(true);
    viewport()->setObjectName("vp");
    viewport()->setStyleSheet(qss_t("vp", bg(Styles::CLEAR)));
    setObjectName("sa");
    setStyleSheet(qss_t("sa", bg(Styles::CLEAR)));
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

void ListWidget::resizeEvent(QResizeEvent *event) {
    QScrollArea::resizeEvent(event);
    hBar->setGeometry(0, height() - 8, width(), 7);
    vBar->setGeometry(width() - 8, 0, 7, height());
    updateListBase();
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
    int oldPos = pos;
    int dy = event->angleDelta().y();
    globalPos -= dy;
    pos -= dy;
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
    if (dy < 0) {   //下行
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
    vBar->setValue(globalPos);
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

void ListWidget::onPostParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    if (widgetTag->contains("row_height", Data::INT)) {
        int height = widgetTag->getInt("row_height");
        handlers << [height](QWidget* target) {
            auto* list = static_cast<ListWidget*>(target);
            list->setRowHeight(height);
        };
    } else {
        throwInFunc("missing tag 'row_height'");
    }
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

void ListWidget::setModel(IListModel *m) {
    if (model == m) {
        return;
    }
    if (model) {
        disconnect(mc);
    }
    model = m;
    if (m) {
        mc = connect(m, &IListModel::sigDataChanged, this, &ListWidget::onDataChanged);
    }
}

void ListWidget::fillA(int begin, bool forceUpdate) {
    if (!model || (!forceUpdate && idxA == begin)) {
        return;
    }
    idxA = begin;
    int end = qMin(begin + areaRowCount, model->length());
    for (int i = 0; begin < end; i++, begin++) {
        setItemData(items[i], begin);
    }
}

void ListWidget::fillB(int begin, bool forceUpdate) {
    if (!model || (!forceUpdate && idxB == begin)) {
        return;
    }
    idxB = begin;
    int end = qMin(begin + areaRowCount, model->length());
    for (int i = areaRowCount; begin < end; i++, begin++) {
        setItemData(items[i], begin);
    }
}

void ListWidget::setItemData(ListItem *item, int idx) {
    auto* d = model->at(idx);
    item->setData(d);
    item->dataIdx = idx;
    if (d) {
        item->syncDataToWidget();
    } else {
        item->clearWidget();
    }
}

void ListWidget::updateMaxGlobalPos() {
    maxGlobalPos = qMax(0, model->length() * rowHeight - viewport()->height());
    vBar->onRangeChanged(0, maxGlobalPos);
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

void ListWidget::setGlobalPos(int gp, bool forceUpdate) {
    if (!forceUpdate && globalPos == gp) {
        return;
    }
    gp = qMin(maxGlobalPos, qMax(0, gp));
    int areaHeight = container->height() >> 1;
    pos = gp % areaHeight;
    int i = gp / rowHeight;
    i -= i % areaRowCount;
    fillA(i, forceUpdate);
    if (pos > posMid) {
        fillB(i + areaRowCount, forceUpdate);
    }
    globalPos = gp;
    verticalScrollBar()->setValue(pos);
    vBar->setValue(globalPos);
}

void ListWidget::onDataChanged(int begin, int end) {
    int rBorder = pos <= posMid ? idxA + areaRowCount : idxA + (areaRowCount << 1);
    updateMaxGlobalPos();   //对于removeLast的情况，进度条会自动调整
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
}
