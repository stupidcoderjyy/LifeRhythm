//
// Created by stupid_coder_jyy on 2024/2/23.
//

#ifndef LIFERHYTHM_RANGEBAR_H
#define LIFERHYTHM_RANGEBAR_H

#include "ScrollArea.h"
#include "Widget.h"
#include "ListData.h"

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
    friend class VRangeItemsContainer;
    friend class HRangeItemsContainer;
    friend class RangeBar;
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

class AbstractRangeWidgetsContainer : public Widget {
    Q_OBJECT
    friend class RangeBar;
protected:
    int minVal;         //最小值
    int maxVal;         //最大值
    double vpp;            //每像素对应的数值
    double maxVpp;         //放大过程中最大可以达到的vpp
    double minVpp;         //缩小过程中最小可以达到的vpp
    int zoomEnabled;    //是否允许缩放
    double zoomStep;       //缩放一次变化的vpp值
    QVector<RangeBarItem*> rangeWidgets{};
protected:
    explicit AbstractRangeWidgetsContainer(QWidget* parent = nullptr);
    virtual void updateBar() = 0;                           //更新容器的大小和内部的RangeWidget
    virtual void updateRangeWidget(RangeBarItem* rw) = 0;    //更新RangeWidget的位置和大小
    void wheelEvent(QWheelEvent *event) override;
signals:
    void sigZoom();
};

class VRangeItemsContainer : public AbstractRangeWidgetsContainer {
    friend class RangeBar;
protected:
    explicit VRangeItemsContainer(QWidget* parent = nullptr);
    void updateBar() override;
    void updateRangeWidget(RangeBarItem *rw) override;
};

class HRangeItemsContainer : public AbstractRangeWidgetsContainer {
    friend class RangeBar;
protected:
    explicit HRangeItemsContainer(QWidget* parent = nullptr);
    void updateBar() override;
    void updateRangeWidget(RangeBarItem *rw) override;
};

class RangeBar : public ScrollArea {
    Q_OBJECT
private:
    AbstractRangeWidgetsContainer* container{};
public:
    explicit RangeBar(bool isVertical, QWidget* parent = nullptr);
    void setData(ListData* d);
    void initPeriodWidget(RangeBarItem* rw);
    void setContainer(AbstractRangeWidgetsContainer* c);    //设置自定义容器
    void updateBar();
    void setBarRange(int minVal, int maxVal);
    void setZoomRange(double minVpp, double maxVpp);
    void setZoomEnabled(bool enabled = true);
    void setZoomStep(double step);
    void setVpp(double vpp);
    void syncDataToWidget() override;
protected:
    virtual RangeBarItem* createRangeWidget();
    void showEvent(QShowEvent *event) override;
    void connectModelView() override;
};

#endif //LIFERHYTHM_RANGEBAR_H
