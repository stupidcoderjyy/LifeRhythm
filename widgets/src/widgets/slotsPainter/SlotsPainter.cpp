//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "SlotsPainter.h"

#include <Helpers.h>
#include <NBT.h>
#include <QMouseEvent>

USING_LR

SlotsPainter::SlotsPainter(QWidget *parent, bool initInConstructor): Widget(parent, initInConstructor), slotWidth(), slotHeight(),
        columns(1), rows(1), vSlotSizePolicy(Auto), hSlotSizePolicy(Auto) {
}

void SlotsPainter::addLayer(SlotsPainterLayer *layer) {
    layer->parent = this;
    layers << layer;
    if (prepared) {
        updateBase();
        update();
    }
}

void SlotsPainter::removeLayer(SlotsPainterLayer* layer) {
    int i = layers.indexOf(layer);
    if (i < 0) {
        return;
    }
    auto* l = layers.takeAt(i);
    l->parent = nullptr;
    if (prepared) {
        update();
    }
}

void SlotsPainter::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    if (widgetTag->contains("slots_count", Data::ARR)) {
        int slotCounts[2];
        widgetTag->get("slots_count")->asArray()->fillInt(slotCounts, 2);
        handlers << [slotCounts](QWidget* t) {
            auto painter = static_cast<SlotsPainter*>(t);
            painter->setSlotCount(slotCounts[0], slotCounts[1]);
        };
    }
    if (widgetTag->contains("slots_size", Data::ARR)) {
        int slotsSize[2];
        widgetTag->get("slots_size")->asArray()->fillInt(slotsSize, 2);
        handlers << [slotsSize](QWidget* t) {
            auto painter = static_cast<SlotsPainter*>(t);
            painter->setSlotSize(slotsSize[0], slotsSize[1]);
        };
    }
    if (widgetTag->contains("slots_size_policy", Data::ARR)) {
        QStringList list;
        widgetTag->get("slots_size_policy")->asArray()->fillString(list, 2);
        SlotSizePolicy policy[2];
        for (int i = 0; i < 2; i++) {
            if (list[i] == "Auto") {
                policy[i] = Auto;
            } else if (list[i] == "Fixed") {
                policy[i] = Fixed;
            }
        }
        handlers << [policy](QWidget* t) {
            auto painter = static_cast<SlotsPainter*>(t);
            painter->setSlotSizePolicy(policy[0], policy[1]);
        };
    }
}

SlotsPainter::~SlotsPainter() {
    DELETE_LIST(layers)
}

void SlotsPainter::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    QRect area;
    for (auto* l : layers) {
        if (!l->shouldDraw()) {
            continue;
        }
        area.setRect(0, 0, slotWidth, slotHeight);
        l->beforeDrawing(p);
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                l->drawSlot(p, area, c, r);
                area.translate(slotWidth, 0);
            }
            area.translate(-area.x(), slotHeight);
        }
        l->afterDrawing(p);
    }
}

void SlotsPainter::resizeEvent(QResizeEvent *event) {
    Widget::resizeEvent(event);
    updateBase();
}

void SlotsPainter::mousePressEvent(QMouseEvent *event) {
    auto p = event->pos();
    int r = p.y() / slotHeight, c = p.x() / slotWidth;
    for (auto l : layers) {
        l->mousePressed(c, r);
    }
    emit sigPressSlot(c, r);
}

void SlotsPainter::mouseReleaseEvent(QMouseEvent *event) {
    auto p = event->pos();
    int r = p.y() / slotHeight, c = p.x() / slotWidth;
    for (auto l : layers) {
        l->mouseReleased(c, r);
    }
    emit sigReleaseSlot(c, r);
}

void SlotsPainter::wheelEvent(QWheelEvent *event) {
    emit sigScroll(event->angleDelta().y());
}

void SlotsPainter::mouseMoveEvent(QMouseEvent *event) {
    auto p = event->pos();
    int r = p.y() / slotHeight, c = p.x() / slotWidth;
    if (prevColumn != c || prevRow != r) {
        prevColumn = c;
        prevRow = r;
        for (auto l: layers) {
            l->mouseEntered(c, r);
        }
        emit sigMouseEntered(c, r);
    }
}

void SlotsPainter::leaveEvent(QEvent *event) {
    for (auto l: layers) {
        l->mouseLeaved();
    }
    prevColumn = prevRow = -1;
}

void SlotsPainter::initWidget() {
    prepared = true;
    updateBase();
}

void SlotsPainter::updateBase() {
    if (prepared) {
        if (vSlotSizePolicy == Auto) {
            slotHeight = height() / rows;
        }
        if (hSlotSizePolicy == Auto) {
            slotWidth = width() / columns;
        }
        std::sort(layers.begin(), layers.end(), [](auto *l, auto *r) {
            return l->zPos < r->zPos;
        });
    }
}

SlotsPainterLayer::SlotsPainterLayer(int z): zPos(z), parent() {
}

SlotsPainterLayer::~SlotsPainterLayer() = default;

bool SlotsPainterLayer::shouldDraw() {
    return true;
}

void SlotsPainterLayer::beforeDrawing(QPainter &p) {
}

void SlotsPainterLayer::drawSlot(QPainter &p, QRect &area, int column, int row) {
}

void SlotsPainterLayer::afterDrawing(QPainter &p) {
}

void SlotsPainterLayer::mousePressed(int column, int row) {
}

void SlotsPainterLayer::mouseReleased(int column, int row) {
}

void SlotsPainterLayer::mouseEntered(int column, int row) {
}

void SlotsPainterLayer::mouseLeaved() {
}

USING_NP(sp_layers)

BackgroundLayer::BackgroundLayer(QColor bgColor): SlotsPainterLayer(-1), bgColor(std::move(bgColor)) {
}

void BackgroundLayer::beforeDrawing(QPainter &p) {
    p.fillRect(parent->rect(), bgColor);
}