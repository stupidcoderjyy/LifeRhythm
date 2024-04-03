//
// Created by stupid_coder_jyy on 2024/4/2.
//

#ifndef LIFERHYTHM_RANGEBARLIST_H
#define LIFERHYTHM_RANGEBARLIST_H

#include "Widget.h"
#include "ScrollArea.h"
#include "RangeBar.h"
#include "Error.h"

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
    QVector<QVector<BarItem*>*> bars;
public:
    inline int getMinVal() const;
    inline int getMaxVal() const;
    inline double getVpp() const;
    inline double getMaxVpp() const;
    inline double getMinVpp() const;
    inline bool isMainZoomEnabled() const;
    inline double getMainZoomStep() const;
    inline bool isSideZoomEnabled() const;
    inline int getSideZoomStep() const;
    inline int getMinSideLen() const;
    inline int getMaxSideLen() const;
    inline int getSideLen() const;
protected:
    explicit AbstractBarListContainer(QWidget* parent = nullptr);
    virtual void updateBarGeometry() = 0;                                   //更新容器及其内部组件的大小
    virtual void updateRangeWidgetGeometry(BarItem* rw) = 0;  //更新RangeWidget的位置和大小
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
    void updateRangeWidgetGeometry(BarItem *rw) override;
};

class HBarListContainer : public AbstractBarListContainer {
    Q_OBJECT
public:
    explicit HBarListContainer(QWidget* parent = nullptr);
protected:
    void updateBarGeometry() override;
    void updateRangeWidgetGeometry(BarItem *rw) override;
    void wheelEvent(QWheelEvent *event) override;
signals:
    void sigScroll(int dx);
};

class RangeBarList : public ScrollArea {
    Q_OBJECT
public:
    enum BarUpdateType {
        Init,
        BarCountChanged,
        MainZoom,
        SideZoom
    };
protected:
    AbstractBarListContainer* container;
    QWidget* rootContent;
    bool assembled;
    bool isVertical;
public:
    explicit RangeBarList(AbstractBarListContainer* container, QWidget* parent = nullptr);
    inline void setBarRange(int minVal, int maxVal);
    inline void setZoomEnabled(bool main, bool side);
    inline void setMainZoomRange(double minVpp, double maxVpp);
    inline void setMainZoomStep(double step);
    inline void setVpp(double vpp);
    inline void setSideZoomStep(int step);
    inline void setSideZoomRange(int minSideLen, int maxSideLen);
    inline void setData(NestedListData* d);
    void syncDataToWidget() override;
signals:
    void sigBarLayoutChanged();
protected:
    void connectModelView() override;
    void resizeEvent(QResizeEvent *event) override;
    virtual BarItem* createRangeWidget();
    virtual void assembleContainer();
    virtual void updateContainerSize();
private:
    void initPeriodWidget(BarItem *rw);
    void barDataChanged(BarUpdateType type);
    void performHorizontalZoom();
    void performVerticalZoom();
};

inline int AbstractBarListContainer::getMinVal() const {
    return minVal;
}

inline int AbstractBarListContainer::getMaxVal() const {
    return maxVal;
}

inline double AbstractBarListContainer::getVpp() const {
    return vpp;
}

inline double AbstractBarListContainer::getMaxVpp() const {
    return maxVpp;
}

inline double AbstractBarListContainer::getMinVpp() const {
    return minVpp;
}

inline bool AbstractBarListContainer::isMainZoomEnabled() const {
    return mainZoomEnabled;
}

inline double AbstractBarListContainer::getMainZoomStep() const {
    return mainZoomStep;
}

inline bool AbstractBarListContainer::isSideZoomEnabled() const {
    return sideZoomEnabled;
}

inline int AbstractBarListContainer::getSideZoomStep() const {
    return sideZoomStep;
}

inline int AbstractBarListContainer::getMinSideLen() const {
    return minSideLen;
}

inline int AbstractBarListContainer::getMaxSideLen() const {
    return maxSideLen;
}

inline int AbstractBarListContainer::getSideLen() const {
    return sideLen;
}

inline void RangeBarList::setData(NestedListData *d) {
    ScrollArea::setData(d);
}

inline void RangeBarList::setBarRange(int minVal, int maxVal) {
    if (minVal >= maxVal) {
        throwInFunc("invalid bar range");
    }
    container->minVal = minVal;
    container->maxVal = maxVal;
}

inline void RangeBarList::setZoomEnabled(bool main, bool side) {
    container->mainZoomEnabled = main;
    container->sideZoomEnabled = side;
}

inline void RangeBarList::setMainZoomRange(double minVpp, double maxVpp) {
    if (minVpp >= maxVpp) {
        throwInFunc("invalid main zoom range");
    }
    container->minVpp = minVpp;
    container->maxVpp = maxVpp;
}

inline void RangeBarList::setMainZoomStep(double step) {
    container->mainZoomStep = step;
}

inline void RangeBarList::setVpp(double vpp) {
    container->vpp = vpp;
}

inline void RangeBarList::setSideZoomStep(int step) {
    container->sideZoomStep = step;
}

inline void RangeBarList::setSideZoomRange(int minSideLen, int maxSideLen) {
    if (minSideLen >= maxSideLen) {
        throwInFunc("invalid side zoom range");
    }
    container->minSideLen = minSideLen;
    container->maxSideLen = maxSideLen;
}

#endif //LIFERHYTHM_RANGEBARLIST_H