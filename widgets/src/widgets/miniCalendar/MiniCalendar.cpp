//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "MiniCalendar.h"
#include <QWheelEvent>
#include <QLayout>
#include <QPainterPath>
#include <QTimer>
#include "WidgetFactoryStorage.h"

USING_LR

#define Day ViewLevel::Day
#define Month ViewLevel::Month
#define Year ViewLevel::Year

MiniCalendar::MiniCalendar(QWidget *parent, bool initInConstructor): Widget(parent, initInConstructor),
        prev(), next(), title(), layerContent(), painterWeekdayTitle(), painters(),
        layerTitle(), bottom(), viewLevel(Day), maxViewLevel(Day) {
    close();
}

void MiniCalendar::loadDate(const QDate &date) {
    switch (viewLevel) {
        case Day: {
            dateTopLeft = QDate(date.year(), date.month(), 1);
            break;
        }
        case Month: {
            dateTopLeft = QDate(date.year(), 1, 1);
            break;
        }
        case Year: {
            dateTopLeft = date;
            break;
        }
    }
    syncWidget();
}

void MiniCalendar::setMaxViewLevel(ViewLevel level) {
    if (prepared && viewLevel >= level) {
        internalSwitchLevel(level);
    }
    maxViewLevel = level;
    syncWidget();
}

void MiniCalendar::setViewLevel(ViewLevel level) {
    internalSwitchLevel(level);
    syncWidget();
}

void MiniCalendar::syncWidget() {
    if (!prepared) {
        return;
    }
    ensureTopLeft();
    viewLevel = qMin(viewLevel, maxViewLevel);
    int mark1{}, mark2{}, firstVal{};
    int mark3 = -1;
    auto today = QDate::currentDate();
    switch (viewLevel) {
        case Day: {
            if (dateTopLeft.day() > 14) {
                auto d1 = dateTopLeft.addMonths(1);
                d1.setDate(d1.year(), d1.month(), 1);
                mark1 = static_cast<int>(dateTopLeft.daysTo(d1));
                mark2 = mark1 + d1.daysInMonth();
                dateTitle = d1;
            } else {
                auto d1 = QDate(dateTopLeft.year(), dateTopLeft.month(), 1);
                mark1 = 0;
                mark2 = d1.daysInMonth() - dateTopLeft.day() + 1;
                dateTitle = d1;
            }
            firstVal = dateTopLeft.day();
            if (auto days = dateTopLeft.daysTo(today); days >= 0 && days < 42) {
                mark3 = days;
            }
            break;
        }
        case Month: {
            if (dateTopLeft.month() > 8) {
                auto d1 = dateTopLeft.addYears(1);
                d1.setDate(d1.year(), 1, 1);
                mark1 = 13 - dateTopLeft.month();
                mark2 = mark1 + 11;
                dateTitle = d1;
            } else {
                mark1 = 0;
                mark2 = 13 - dateTopLeft.month();
                dateTitle = dateTopLeft;
            }
            firstVal = dateTopLeft.month();
            if (today >= dateTopLeft && today < dateTopLeft.addMonths(16)) {
                if (today.year() == dateTopLeft.year()) {
                    mark3 = today.month() - dateTopLeft.month();
                } else {
                    mark3 = 12 + today.month() - dateTopLeft.month();
                }
            }
            break;
        }
        case Year: {
            int i = dateTopLeft.year() % 10;
            if (i >= 4) {
                auto d1 = dateTopLeft.addYears(10 - i);
                mark1 = 10 - i;
                mark2 = mark1 + 10;
                dateTitle = d1;
            } else {
                mark1 = 0;
                mark2 = 10 - i;
                dateTitle = dateTopLeft;
            }
            firstVal = dateTopLeft.year();
            auto end = dateTopLeft.addYears(16);
            if (today >= dateTopLeft && today < end) {
                mark3 = today.year() - dateTopLeft.year();
            }
            break;
        }
    }
    layerContent->setVal(maxViewLevel, viewLevel, firstVal, mark1, mark2, mark3);
    updateTitle();
}

#define TIMES 9
#define SLOT_SIZE_1 4 * TIMES
#define SLOT_SIZE_2 7 * TIMES
#define WIDTH 7 * SLOT_SIZE_1
#define HEIGHT 8 * SLOT_SIZE_1

void MiniCalendar::initWidget() {
    if (prepared) {
        return;
    }
    setFixedSize(WIDTH, HEIGHT);    //252x288
    WidgetFactoryStorage::get("test:widget_mini_calendar")->apply(nullptr, this);
    prev = getPointer<ArrowButton>("prev");
    connect(prev, &ArrowButton::sigSelected, this, [this] {
        handleArrowButton(false);
    });
    next = getPointer<ArrowButton>("next");
    connect(next, &ArrowButton::sigSelected, this, [this] {
        handleArrowButton(true);
    });
    title = getPointer<Button>("title");
    connect(title, &Button::sigActivated, this, [this] {
        handleButtonTitle();
    });
    getPointer<Widget>("top")->setFixedHeight(SLOT_SIZE_1);
    layerTitle = new TitleLayer;
    layerContent = new ContentLayer;
    painterWeekdayTitle = new SlotsPainter(this);
    painterWeekdayTitle->setFixedSize(WIDTH, SLOT_SIZE_1);
    painterWeekdayTitle->setSlotCount(7, 1);
    painterWeekdayTitle->addLayer(layerTitle);

    auto* painterDay = new SlotsPainter(this);
    painterDay->setFixedSize(WIDTH, SLOT_SIZE_1 * 6);
    painterDay->setSlotCount(7, 6);
    painterDay->addLayer(new sp_layers::BackgroundLayer(Styles::BLACK->color));
    painterDay->addLayer(layerContent);
    connect(painterDay, &SlotsPainter::sigScroll, this, [this](int dy){
        handlePainterScroll(dy < 0);
    });
    connect(painterDay, &SlotsPainter::sigPressSlot, this, [this](int c, int r) {
        emit sigPress(dateTopLeft.addDays(r * 7 + c));
    });
    connect(painterDay, &SlotsPainter::sigReleaseSlot, this, [this](int c, int r) {
        emit sigRelease(dateTopLeft.addDays(r * 7 + c));
    });

    auto *painterMonth = new SlotsPainter(this);
    painterMonth->setFixedSize(WIDTH, SLOT_SIZE_2 * 4);
    painterMonth->setSlotCount(4, 4);
    painterMonth->addLayer(new sp_layers::BackgroundLayer(Styles::BLACK->color));
    painterMonth->close();
    connect(painterMonth, &SlotsPainter::sigReleaseSlot, this, [this](int c, int r) {
        auto d = dateTopLeft.addMonths((r << 2) + c);
        emit sigRelease(d);
        if (maxViewLevel == Day) {
            dateTopLeft = d;
            internalSwitchLevel(Day);
            syncWidget();
        }
    });
    connect(painterMonth, &SlotsPainter::sigPressSlot, this, [this](int c, int r) {
        emit sigPress(dateTopLeft.addMonths((r << 2) + c));
    });
    connect(painterMonth, &SlotsPainter::sigScroll, this, [this](int dy){
        handlePainterScroll(dy < 0);
    });

    auto *painterYear = new SlotsPainter(this);
    painterYear->setFixedSize(WIDTH, SLOT_SIZE_2 * 4);
    painterYear->setSlotCount(4, 4);
    painterYear->close();
    painterYear->addLayer(new sp_layers::BackgroundLayer(Styles::BLACK->color));
    connect(painterYear, &SlotsPainter::sigReleaseSlot, this, [this](int c, int r) {
        auto d = dateTopLeft.addYears((r << 2) + c);
        emit sigRelease(d);
        if (maxViewLevel > Year) {
            dateTopLeft = d;
            internalSwitchLevel(Month);
            syncWidget();
        }
    });
    connect(painterYear, &SlotsPainter::sigPressSlot, this, [this](int c, int r) {
        emit sigPress(dateTopLeft.addYears((r << 2) + c));
    });
    connect(painterYear, &SlotsPainter::sigScroll, this, [this](int dy){
        handlePainterScroll(dy < 0);
    });

    painters[Day] = painterDay;
    painters[Month] = painterMonth;
    painters[Year] = painterYear;

    bottom = static_cast<QVBoxLayout*>(getPointer<Widget>("bottom")->layout());
    bottom->addWidget(painterWeekdayTitle);
    bottom->addWidget(painterDay);
    ViewLevel l = qMin(viewLevel, maxViewLevel);
    if (viewLevel != l) {
        internalSwitchLevel(l);
    }
    prepared = true;
    QTimer::singleShot(0, [this] {
        loadDate(QDate::currentDate()); //初始化阶段不允许同步数据
    });
    show();
}

void MiniCalendar::internalSwitchLevel(ViewLevel levelNew) {
    if (levelNew == viewLevel) {
        return;
    }
    title->setButtonStyleEnabled(levelNew != Year);
    title->setWidgetEnabled(levelNew != Year);
    if (viewLevel == Day) {
        bottom->removeWidget(painterWeekdayTitle);
        painterWeekdayTitle->close();
    }
    painters[viewLevel]->close();
    painters[viewLevel]->removeLayer(layerContent);
    if (levelNew == Day) {
        bottom->replaceWidget(painters[viewLevel], painterWeekdayTitle);
        bottom->addWidget(painters[Day]);
        painterWeekdayTitle->show();
    } else {
        bottom->replaceWidget(painters[viewLevel], painters[levelNew]);
    }
    painters[levelNew]->addLayer(layerContent);
    painters[levelNew]->show();
    viewLevel = levelNew;
    switch (levelNew) {
        case Day: {
            dateTopLeft.setDate(dateTopLeft.year(), dateTopLeft.month(), 1);
            break;
        }
        case Month: {
            dateTopLeft.setDate(dateTopLeft.year(), 1, 1);
            break;
        }
        default: {
            dateTopLeft.setDate(dateTopLeft.year() - dateTopLeft.year() % 10, 1, 1);
            break;
        }
    }
}

void MiniCalendar::updateTitle() const {
    switch (viewLevel) {
        case Day: {
            title->setButtonText(dateTitle.toString("yyyy年M月"));
            break;
        }
        case Month: {
            title->setButtonText(dateTitle.toString("yyyy年"));
            break;
        }
        case Year: {
            int y = dateTitle.year();
            y = y - y % 10;
            title->setButtonText(QString("%1 - %2").arg(y).arg(y + 9));
            break;
        }
    }
}

void MiniCalendar::handleArrowButton(bool add) {
    switch (viewLevel) {
        case Day: {
            dateTopLeft = dateTitle.addMonths(add ? 1 : -1);
            dateTopLeft.setDate(dateTopLeft.year(), dateTopLeft.month(), 1);
            break;
        }
        case Month: {
            dateTopLeft = dateTitle.addYears(add ? 1 : -1);
            dateTopLeft.setDate(dateTopLeft.year(), 1, 1);
            break;
        }
        case Year: {
            dateTopLeft = dateTitle.addYears(add ? 10 : -10);
            break;
        }
    }
    syncWidget();
}

void MiniCalendar::handleButtonTitle() {
    switch (viewLevel) {
        case Day:
            internalSwitchLevel(Month);
            break;
        case Month:
            internalSwitchLevel(Year);
            break;
        default:
            break;
    }
    syncWidget();
}

void MiniCalendar::handlePainterScroll(bool add) {
    switch (viewLevel) {
        case Day:
            dateTopLeft = dateTopLeft.addDays(add ? 7 : -7);
            break;
        case Month:
            dateTopLeft = dateTopLeft.addMonths(add ? 4 : -4);
            break;
        case Year:
            dateTopLeft = dateTopLeft.addYears(add ? 4 : -4);
            break;
    }
    syncWidget();
}

void MiniCalendar::ensureTopLeft() {
    switch (viewLevel) {
        case Day: {
            dateTopLeft = dateTopLeft.addDays(1 - dateTopLeft.dayOfWeek());
            break;
        }
        case Month: {
            dateTopLeft = dateTopLeft.addMonths(-(dateTopLeft.month() - 1 & 0x3));
            break;
        }
        default: {
            dateTopLeft.setDate(dateTopLeft.year() & 0xFFFC, 1, 1);
            break;
        }
    }
}

USING_NP(mini_calendar)

ContentLayer::ContentLayer(): SlotsPainterLayer(INT32_MAX), firstVal(), mark1(-1), mark2(-1), mark3(-1), mark4(-1), val(), count(), rx(), ry(),
                              viewLevel(), maxViewLevel() {
}

void ContentLayer::setVal(ViewLevel mvl, ViewLevel vl, int fv, int m1, int m2, int m3) {
    maxViewLevel = mvl;
    viewLevel = vl;
    firstVal = fv;
    mark1 = m1;
    mark2 = m2;
    mark3 = m3;
    mark4 = -1;
    if (vl == Day) {
        rx = SLOT_SIZE_1 >> 1;
        ry = 32;
    } else {
        rx = SLOT_SIZE_2 >> 1;
        ry = 50;
    }
    parent->update();
}

void ContentLayer::beforeDrawing(QPainter &p) {
    p.setFont(Styles::FONT_MAIN);
    p.setPen(Styles::GRAY_4->color);
    val = firstVal;
    count = 0;
}

void ContentLayer::drawSlot(QPainter &p, QRect &area, int column, int row) {
    if (count == mark4) {
        p.fillRect(area, Styles::GRAY_1->color);
    }
    if (count == mark1) {
        p.setPen(Styles::GRAY_TEXT_1->color);
        if (viewLevel != Year && mark1 > 0) {
            val = 1;
        }
    } else if (count == mark2) {
        p.setPen(Styles::GRAY_3->color);
        if (viewLevel != Year) {
            val = 1;
        }
    } else if (count == mark3) {
        QPainterPath path;
        QPoint center(area.x() + rx,area.y() + ry);
        path.addEllipse(center, 2, 2);
        p.fillPath(path, Styles::BLUE_1->color);
    }
    if (viewLevel == Month) {
        p.drawText(area, Qt::AlignCenter, QString::number(val++) + "月");
    } else {
        p.drawText(area, Qt::AlignCenter, QString::number(val++));
    }
    count++;
}

void ContentLayer::mousePressed(int column, int row) {
    if (viewLevel != maxViewLevel) {
        mark4 = row * 4 + column;
        parent->update();
    }
}

void TitleLayer::beforeDrawing(QPainter &p) {
    p.setFont(FontBuilder(Styles::FONT_TYPE_MAIN, Styles::FONT_MAIN).setBoldWeight().get());
    p.fillRect(parent->rect(), Styles::BLACK->color);
    p.setPen(Styles::GRAY_TEXT_1->color);
}

void TitleLayer::drawSlot(QPainter &p, QRect &area, int column, int row) {
    p.drawText(area, Qt::AlignCenter, Helpers::WEEKDAYS_CN[column]);
}
