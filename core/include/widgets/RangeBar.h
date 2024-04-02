//
// Created by stupid_coder_jyy on 2024/2/23.
//

#ifndef LIFERHYTHM_RANGEBAR_H
#define LIFERHYTHM_RANGEBAR_H

#include "ScrollArea.h"
#include "Widget.h"
#include "ListData.h"

enum BarUpdateType {
    Content,
    Zoom
};

class RangeBarData : public WidgetData {
    Q_OBJECT
    friend class RangeBarItem;
protected:
    int begin;
    int end;
public:
    RangeBarData();
    int getBegin() const;
    int getEnd() const;
    void setBegin(int begin);
    void setEnd(int end);
};

class RangeBarItem : public Widget {
    Q_OBJECT
    friend class VBarContainer;
    friend class HBarContainer;
    friend class RangeBar;
    friend class VBarListContainer;
    friend class HBarListContainer;
protected:
    int begin;
    int end;
public:
    explicit RangeBarItem(QWidget* parent = nullptr);
    void syncDataToWidget() override;
    void syncWidgetToData() override;
signals:
    void sigUpdateWidget();
};

class AbstractBarContainer : public Widget {
    Q_OBJECT
    friend class RangeBar;
protected:
    int minVal;         //最小值
    int maxVal;         //最大值
    double vpp;            //每像素对应的数值
    double maxVpp;         //放大过程中最大可以达到的vpp
    double minVpp;         //缩小过程中最小可以达到的vpp
    bool zoomEnabled;    //是否允许缩放
    double zoomStep;       //缩放一次变化的vpp值
    QVector<RangeBarItem*> rangeWidgets;
protected:
    explicit AbstractBarContainer(QWidget* parent = nullptr);
    virtual void updateBarGeometry() = 0;                           //更新容器及其内部组件的大小
    virtual void updateRangeWidgetGeometry(RangeBarItem* rw) = 0;   //更新RangeWidget的位置和大小
    void wheelEvent(QWheelEvent *event) override;
signals:
    void sigZoom();
};

class VBarContainer : public AbstractBarContainer {
    friend class RangeBar;
protected:
    explicit VBarContainer(QWidget* parent = nullptr);
    void updateBarGeometry() override;
    void updateRangeWidgetGeometry(RangeBarItem *rw) override;
};

class HBarContainer : public AbstractBarContainer {
    friend class RangeBar;
protected:
    explicit HBarContainer(QWidget* parent = nullptr);
    void updateBarGeometry() override;
    void updateRangeWidgetGeometry(RangeBarItem *rw) override;
};

class RangeBar : public ScrollArea {
    Q_OBJECT
protected:
    bool assembled;
    bool isVertical;
    QWidget* rootContent;
    AbstractBarContainer* container;
public:
    explicit RangeBar(AbstractBarContainer* c, QWidget* parent = nullptr);
    void setData(ListData* d);
    void setBarRange(int minVal, int maxVal);
    void setZoomRange(double minVpp, double maxVpp);
    void setZoomEnabled(bool enabled = true);
    void setZoomStep(double step);
    void setVpp(double vpp);
    void syncDataToWidget() override;
signals:
    void sigBarDataChanged(double vpp);
protected:
    void initPeriodWidget(RangeBarItem* rw);
    virtual RangeBarItem* createRangeWidget();
    virtual void assembleContainer();
    virtual void updateContainerSize();
    void barDataChanged(BarUpdateType type);
    void connectModelView() override;
    void resizeEvent(QResizeEvent *event) override;
    void initBar();
};

#endif //LIFERHYTHM_RANGEBAR_H
