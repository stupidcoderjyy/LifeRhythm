//
// Created by stupid_coder_jyy on 2024/2/23.
//

#ifndef LIFERHYTHM_RANGEBAR_H
#define LIFERHYTHM_RANGEBAR_H

#include "ScrollArea.h"
#include "Widget.h"

class RangeWidgetData : public QObject{
    Q_OBJECT
    friend class RangeWidget;
protected:
    int begin;
    int end;
public:
    explicit RangeWidgetData(QObject* parent = nullptr);
    int getBegin() const;
    int getEnd() const;
    void setBegin(int begin);
    void setEnd(int end);
signals:
    void sigDataChanged();
};

class RangeWidget : public Widget {
    Q_OBJECT
    friend class VRangeWidgetsContainer;
    friend class HRangeWidgetsContainer;
    friend class RangeBar;
protected:
    int begin;
    int end;
    RangeWidgetData* data;
public:
    explicit RangeWidget(QWidget* parent = nullptr);
    void setData(RangeWidgetData* data);
    virtual void syncDataToWidget();       //将RangeWidgetData中的数据载入组件（触发sigRangeChanged）
    virtual void syncWidgetToData();
    template<class DATA> DATA* castedData() {
        return static_cast<DATA*>(data);
    }
signals:
    void sigRangeChanged(int begin, int end);           //RangeWidget范围被修改后触发此信号
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
    QVector<RangeWidget*> rangeWidgets{};
protected:
    explicit AbstractRangeWidgetsContainer(QWidget* parent = nullptr);
    virtual void updateBar() = 0;                           //更新容器的大小和内部的RangeWidget
    virtual void updateRangeWidget(RangeWidget* rw) = 0;    //更新RangeWidget的位置和大小
    void wheelEvent(QWheelEvent *event) override;
signals:
    void sigZoom();
};

class VRangeWidgetsContainer : public AbstractRangeWidgetsContainer {
    friend class RangeBar;
protected:
    explicit VRangeWidgetsContainer(QWidget* parent = nullptr);
    void updateBar() override;
    void updateRangeWidget(RangeWidget *rw) override;
};

class HRangeWidgetsContainer : public AbstractRangeWidgetsContainer {
    friend class RangeBar;
protected:
    explicit HRangeWidgetsContainer(QWidget* parent = nullptr);
    void updateBar() override;
    void updateRangeWidget(RangeWidget *rw) override;
};

class RangeBar : public ScrollArea {
private:
    AbstractRangeWidgetsContainer* container{};
public:
    explicit RangeBar(bool isVertical, QWidget* parent = nullptr);
    RangeWidget* addPeriod(RangeWidgetData* customData = nullptr);
    void setContainer(AbstractRangeWidgetsContainer* c);    //设置自定义容器
    void updateBar();
    void setBarRange(int minVal, int maxVal);
    void setZoomRange(double minVpp, double maxVpp);
    void setZoomEnabled(bool enabled = true);
    void setZoomStep(double step);
    void setVpp(double vpp);
protected:
    virtual RangeWidget* createRangeWidget();
    void showEvent(QShowEvent *event) override;
};

#endif //LIFERHYTHM_RANGEBAR_H
