//
// Created by stupid_coder_jyy on 2024/4/2.
//

#ifndef LIFERHYTHM_RANGEBARLIST_H
#define LIFERHYTHM_RANGEBARLIST_H

#include "RangeBar.h"

class AbstractBarListContainer : public QWidget {
    Q_OBJECT
    friend class RangeBarList;
protected:
    int minVal;             //最小值
    int maxVal;             //最大值
    double vpp;             //每像素对应的数值
    double maxVpp;          //放大过程中最大可以达到的vpp
    double minVpp;          //缩小过程中最小可以达到的vpp
    bool mainZoomEnabled;   //是否允许主方向缩放（改变条的长度）
    double mainZoomStep;    //缩放一次变化的vpp值
    bool sideZoomEnabled;   //是否允许侧方向缩放（改变条的宽度）
    int sideZoomStep;       //侧方向缩放步长
    int minSideLen;         //侧方向缩放过程中每个条的最大侧边长度
    int maxSideLen;         //侧方向缩放过程中每个条的最小侧边长度
    int sideLen;            //当前每个条的侧边长度
    QVector<QVector<RangeBarItem*>> bars;
protected:
    explicit AbstractBarListContainer(QWidget* parent = nullptr);
    virtual void updateBarGeometry() = 0;                                   //更新容器及其内部组件的大小
    virtual void updateRangeWidgetGeometry(int bar, RangeBarItem* rw) = 0;  //更新RangeWidget的位置和大小
    void wheelEvent(QWheelEvent *event) override;
signals:
    void sigMainZoom();
    void sigSideZoom();
};

class VBarListContainer : public AbstractBarListContainer {
public:
    explicit VBarListContainer(QWidget* parent = nullptr);
protected:
    void updateBarGeometry() override;
    void updateRangeWidgetGeometry(int bar, RangeBarItem *rw) override;
};

class HBarListContainer : public AbstractBarListContainer {
public:
    explicit HBarListContainer(QWidget* parent = nullptr);
protected:
    void updateBarGeometry() override;
    void updateRangeWidgetGeometry(int bar, RangeBarItem *rw) override;
};

class RangeBarList : public ScrollArea {
protected:
    AbstractBarListContainer* container;
public:
    explicit RangeBarList(AbstractBarListContainer* container, QWidget* parent = nullptr);
    void setBarRange(int minVal, int maxVal);
    void setZoomEnabled(bool main, bool side);
    void setMainZoomRange(double minVpp, double maxVpp);
    void setMainZoomStep(double step);
    void setVpp(double vpp);
    void setSideZoomStep(int step);
    void setSideZoomRange(int minSideLen, int maxSideLen);
};


#endif //LIFERHYTHM_RANGEBARLIST_H
