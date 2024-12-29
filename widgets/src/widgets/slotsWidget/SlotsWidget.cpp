//
// Created by JYY on 24-11-4.
//

#include "SlotsWidget.h"
#include "NBT.h"
#include "WidgetFactory.h"

USING_LR

SlotsWidget::SlotsWidget(QWidget *parent, bool iic): Widget(parent, iic), slotWidth(50), slotHeight(50), columns(0), rows(0),
        vSlotSizePolicy(Auto), hSlotSizePolicy(Auto), factoryItem() {
}

void SlotsWidget::setData(WidgetData *d) {
    if (!prepared) {
        updateBase();
    }
    Widget::setData(d);
    syncItems(0, INT32_MAX);    //update all
}

void SlotsWidget::setItemFactory(WidgetFactory *factory) {
    if (factoryItem == factory) {
        return;
    }
    if (factoryItem) {
        delete factoryItem;
    }
    factoryItem = factory;
}

SlotsWidget::~SlotsWidget() {
    delete factoryItem;
}

void SlotsWidget::prepareItem(ListItem *w) {
    w->setParent(this);
    w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    w->initWidget();    //确保ListItem处于已加载的状态
    w->show();
    items << w;
}

void SlotsWidget::syncItem(int idx) const {
    auto item = items[idx];
    if (auto ld = wData->cast<ListData>()) {
        auto d = ld->at(idx);
        item->setData(d);
        item->dataIdx = idx;
        item->setList(ld);
    } else {
        item->setData(nullptr);
        item->dataIdx = -1;
        item->setList(nullptr);
    }
    item->syncDataToWidget();
}

ListItem * SlotsWidget::newItem() {
    if (factoryItem) {
        return factoryItem->applyAndCast<ListItem>();
    }
    return new ListItem();
}

void SlotsWidget::updateBase() {
    if (rows == 0 || columns == 0) {
        return;
    }
    prepared = true;
    int oldSlotHeight = slotHeight, oldSlotWidth = slotWidth;
    if (vSlotSizePolicy == Auto) {
        slotHeight = (height() - regionMargins.top() - regionMargins.bottom()) / rows;
    }
    if (hSlotSizePolicy == Auto) {
        slotWidth = (width() - regionMargins.right() - regionMargins.left()) / columns;
    }
    if (oldSlotHeight == slotHeight && oldSlotWidth == slotWidth) {
        return;
    }
    region = {slotWidth * columns, slotHeight * rows};
    int totalItems = rows * columns;
    items.reserve(totalItems);
    int oldItems = items.length();
    for (int i = oldItems; i < totalItems; i++) {
        prepareItem(newItem());
    }
    int i = 0, x = regionMargins.left(), y = regionMargins.top();
    for (int r = 0; r < rows; r++, x = regionMargins.left(), y += slotHeight) {
        for (int c = 0; c < columns; c++) {
            auto item = items[i++];
            item->setGeometry(QRect(x, y, slotWidth, slotHeight).marginsRemoved(slotMargins));
            item->setVisible(true);
            x += slotWidth;
        }
    }
    int rb = i, rn = items.length() - i;
    while (i < items.length()) {
        delete items[i++];
    }
    items.remove(rb, rn);
}

void SlotsWidget::syncItems(int begin, int end) const {
    int rBorder = rows * columns;
    if (end < 0 || begin >= rBorder) {
        return;
    }
    begin = qMax(0, begin);
    end = qMin(rBorder - 1, end);
    while (begin <= end) {
        syncItem(begin++);
    }
}

void SlotsWidget::resizeEvent(QResizeEvent *event) {
    Widget::resizeEvent(event);
    updateBase();
}

void SlotsWidget::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this] {
        if (!prepared) {
            updateBase();
        }
        auto* d = wData->cast<ListData>();
        syncItems(d->getChangeBegin(), d->getChangeEnd());
    });
}

void SlotsWidget::onPostParsing(Handlers &handlers, NBT *nbt) {
    if (nbt->contains("item", Data::COMPOUND)) {
        auto *f = WidgetFactory::fromNbt("item", nbt->get("item")->asCompound());
        f->include(WidgetFactory::factoryInParse());
        try {
            f->parse();
        } catch (Error &) {
            delete f;
            throw;
        }
        handlers << [f](QWidget *widget) {
            auto *l = static_cast<SlotsWidget*>(widget);
            if (l->factoryItem) {
                delete f;
            } else {
                l->factoryItem = f;
            }
        };
    }
}