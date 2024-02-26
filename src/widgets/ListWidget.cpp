//
// Created by stupid_coder_jyy on 2024/2/25.
//

#include "ListWidget.h"
#include "Styles.h"
#include "ScrollBar.h"
#include "Error.h"
#include "NBT.h"

ListItem::ListItem(QWidget *parent): Widget(parent),
        data(), dc(), dataIdx() {
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

ListWidget::ListWidget(QWidget *parent): QScrollArea(parent), StandardWidget(), rowHeight(40),
        areaRowCount(), container(new QWidget(this)), pos(), items(), model(), posBottom(),
        posMid(), idxA(-1), idxB(-1), globalPos(), maxGlobalPos() {
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
    int newRowCount = viewport()->height() / rowHeight + 5;
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
            appendItem();
        }
    } else {
        int oldCount = items.length();
        //补上缺失的item
        items.reserve(itemsCount);
        for (int i = oldCount ; i < itemsCount; i++) {
            appendItem();
        }
    }
    setGlobalPos(globalPos, true);
}

void ListWidget::appendItem() {
    auto* w = createRowItem();
    w->setParent(container);
    w->setFixedHeight(rowHeight);
    w->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    layout->addWidget(w);
    items << w;
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

void ListWidget::fillA(int begin, bool force) {
    if (!model || (!force && idxA == begin)) {
        return;
    }
    idxA = begin;
    int end = qMin(begin + areaRowCount, model->length());
    for (int i = 0; begin < end; i++, begin++) {
        setItemData(items[i], begin);
    }
}

void ListWidget::fillB(int begin, bool force) {
    if (!model || (!force && idxB == begin)) {
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

void ListWidget::setGlobalPos(int gp, bool force) {
    if (!force && globalPos == gp) {
        return;
    }
    gp = qMin(maxGlobalPos, qMax(0, gp));
    int areaHeight = container->height() >> 1;
    pos = gp % areaHeight;
    int i = gp / rowHeight;
    i -= i % areaRowCount;
    fillA(i, force);
    if (pos > posMid) {
        fillB(i + areaRowCount, force);
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
    int i = qMax(begin, idxA) - idxA;
    int j = qMin(end, rBorder) - idxA;
    while (i <= j) {
        setItemData(items[i], begin);
        i++;
        begin++;
    }
}
