//
// Created by stupid_coder_jyy on 2024/3/21.
//

#include "ColorSelector.h"
#include "Styles.h"
#include "RcManagers.h"
#include "WidgetDataStorage.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

VColorBar::VColorBar(QWidget *parent): Widget(parent), val(-1) {
    barImg = ImageStorage::get("lr:v_color_bar");
    setFixedSize(barImg->size());
    step = 360 / barImg->height();
}

void VColorBar::selectColor(const QColor &c) {
    onColorSelected(c.hue() / step);
}

void VColorBar::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, *barImg);
    painter.setPen(Styles::GRAY_TEXT_0->color);
    painter.drawLine(0, val - 1, width(), val - 1);
    painter.drawLine(0, val + 1, width(), val + 1);
    painter.setPen(Styles::BLACK->color);
    painter.drawLine(0, val, width(), val);
}

void VColorBar::mousePressEvent(QMouseEvent *event) {
    pressed = true;
    onColorSelected(qMin(height(), qMax(0, event->y())));
}

void VColorBar::mouseReleaseEvent(QMouseEvent *event) {
    pressed = false;
}

void VColorBar::mouseMoveEvent(QMouseEvent *event) {
    if (pressed) {
        onColorSelected(qMin(height(), qMax(0, event->y())));
    }
}

QColor VColorBar::getColor() const {
    int h = val * step;
    if (h == 360) {
        h = 0;
    }
    return QColor::fromHsv(h, 0xFF, 0xFF);
}

void VColorBar::onColorSelected(int v) {
    if (val != v) {
        val = v;
        emit sigColorSelected(getColor());
        update();
    }
}

ColorBarImageGenerator::ColorBarImageGenerator(): isVertical(), step(1), barWidth(20), img() {
}

void ColorBarImageGenerator::setBarData(bool v, int s, int bw) {
    isVertical = v;
    step = s;
    barWidth = bw;
    updateAll();
}

void ColorBarImageGenerator::outputImg(const QString &path) {
    if (!img) {
        updateAll();
    }
    QPainter painter(img);
    QColor color;
    pos = 0;
    int h = 0;
    while (h <= 360) {
        color.setHsv(h, 255, 255);
        if (isVertical) {
            painter.fillRect(0, pos++, img->width(), 1, color);
        } else {
            painter.fillRect(pos++, 0, 1, img->height(), color);
        }
        h += step;
    }
    img->save(path);
}

ColorBarImageGenerator::~ColorBarImageGenerator() {
    delete img;
}

void ColorBarImageGenerator::updateAll() {
    delete img;
    int len = 360 / step;
    if (isVertical) {
        img = new QPixmap(barWidth, len);
    } else {
        img = new QPixmap(len, barWidth);
    }
}

ColorIcon::ColorIcon(QWidget *parent): Widget(parent), bdWidth(2), bdColor(Styles::GRAY_TEXT_0->color) {
    setFixedSize(20, 20);
}

void ColorIcon::paintEvent(QPaintEvent *event) {
    if (wData) {
        QPainter painter(this);
        QRect r = rect();
        if (bdWidth > 0) {
            painter.fillRect(r, bdColor);
        }
        painter.fillRect(r.adjusted(bdWidth, bdWidth, -bdWidth, -bdWidth), wData->cast<Color>()->color);
    }
}

void ColorIcon::onPostParsing(StandardWidget::Handlers &handlers, NBT *nbt) {
    if (nbt->contains("border_width", Data::INT)) {
        int w = nbt->getInt("border_width");
        handlers << [w](QWidget* target) {
            static_cast<ColorIcon*>(target)->bdWidth = w;
        };
    }
    if (nbt->contains("border_color", Data::STRING)) {
        QColor c = Styles::parseColor(nbt->getString("border_color"));
        handlers << [c](QWidget* target) {
            static_cast<ColorIcon*>(target)->bdColor = c;
        };
    }
}

void ColorIcon::setBorderWidth(int w) {
    bdWidth = w;
}

void ColorIcon::setBorderColor(QColor c) {
    bdColor = std::move(c);
}

DefaultColorsListItem::DefaultColorsListItem(QWidget *parent): SelectableListItem(parent), icon(), labelName() {
}

void DefaultColorsListItem::init() {
    icon = getPointer<ColorIcon>("icon");
    labelName = getPointer<TextLabel>("name");
}

void DefaultColorsListItem::syncDataToWidget() {
    SelectableListItem::syncDataToWidget();
    icon->setData(wData);
    labelName->setText(wData ? wData->cast<Color>()->name : "");
    setState(selected);
}

void DefaultColorsListItem::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<DefaultColorsListItem*>(target)->init();
    };
}

DefaultColorsList::DefaultColorsList(QWidget *parent): SelectableListWidget(parent) {
    setRowHeight(30);
}

SelectableListItem *DefaultColorsList::createRowItem() {
    return WidgetFactoryStorage::get("lr:item_default_colors")->applyAndCast<SelectableListItem>();
}

HueSelector::HueSelector(QWidget *parent): Widget(parent) {
}

void HueSelector::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<HueSelector*>(target)->init();
    };
}

void HueSelector::init() {
    bar = getPointer<VColorBar>("bar");
    auto* modelColors = WidgetDataStorage::get("lr:default_colors")->cast<SelectableListData>();
    connect(modelColors, &SelectableListData::sigDataSelected, this, [this, modelColors](int pre, int cur){
        auto* c = modelColors->at(cur)->cast<Color>();
        bar->selectColor(c->color);
    });
    connect(bar, &VColorBar::sigColorSelected, this, &HueSelector::sigColorSelected);
}
