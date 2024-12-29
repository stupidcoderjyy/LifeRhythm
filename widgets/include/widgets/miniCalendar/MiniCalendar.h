//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef MINICALENDAR_H
#define MINICALENDAR_H

#include <Namespaces.h>
#include <qboxlayout.h>
#include "Widget.h"
#include "SlotsPainter.h"
#include <QDateTime>
#include "ArrowButton.h"

BEGIN_LR

BEGIN_NP(mini_calendar)

class ContentLayer;
class TitleLayer;

enum ViewLevel {
    Year,
    Month,
    Day
};

END_NP

class MiniCalendar : public Widget {
    Q_OBJECT
public:
    using ContentLayer = mini_calendar::ContentLayer;
    using TitleLayer = mini_calendar::TitleLayer;
    using ViewLevel = mini_calendar::ViewLevel;
protected:
    ArrowButton* prev;
    ArrowButton* next;
    Button* title;
    ContentLayer* layerContent;
    SlotsPainter* painterWeekdayTitle;
    SlotsPainter* painters[3];
    TitleLayer *layerTitle;
    QVBoxLayout *bottom;
    ViewLevel viewLevel;
    ViewLevel maxViewLevel;
    QDate dateTopLeft;
    QDate dateTitle;
public:
    explicit MiniCalendar(QWidget* parent = nullptr, bool initInConstructor = true);
    void loadDate(const QDate& date);
    void setMaxViewLevel(ViewLevel level);
    void setViewLevel(ViewLevel level);
    inline ViewLevel getViewLevel() const;
signals:
    void sigPress(const QDate& date);
    void sigRelease(const QDate& date);
protected:
    virtual void syncWidget();
    void initWidget() override;
private:
    void internalSwitchLevel(ViewLevel levelNew);
    void updateTitle() const;
    void handleArrowButton(bool add);
    void handleButtonTitle();
    void handlePainterScroll(bool add);
    void ensureTopLeft();
};

inline MiniCalendar::ViewLevel MiniCalendar::getViewLevel() const {
    return viewLevel;
}

BEGIN_NP(mini_calendar)

class ContentLayer : public SlotsPainterLayer {
private:
    int firstVal;
    int mark1, mark2, mark3, mark4;
    int val, count;
    int rx, ry;
    ViewLevel viewLevel;
    ViewLevel maxViewLevel;
public:
    ContentLayer();
    void setVal(ViewLevel maxViewLevel, ViewLevel viewLevel, int firstVal, int mark1, int mark2, int mark3);
protected:
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int column, int row) override;
    void mousePressed(int column, int row) override;
};

class TitleLayer : public SlotsPainterLayer {
protected:
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int column, int row) override;
};

END_NP

END_LR

#endif //MINICALENDAR_H
