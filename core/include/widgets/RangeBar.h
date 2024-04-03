//
// Created by stupid_coder_jyy on 2024/2/23.
//

#ifndef LIFERHYTHM_RANGEBAR_H
#define LIFERHYTHM_RANGEBAR_H

#include "ScrollArea.h"
#include "Widget.h"
#include "ListData.h"
#include "NestedListData.h"
#include "Error.h"

class BarData : public NestedListNode {
    Q_OBJECT
    friend class BarItem;
protected:
    int begin;
    int end;
public:
    BarData();
    BarData(int begin, int end);
    inline int getBegin() const;
    inline int getEnd() const;
    inline void setBegin(int begin);
    inline void setEnd(int end);
};

class BarItem : public Widget {
Q_OBJECT
    friend class VBarListContainer;
    friend class HBarListContainer;
    friend class VBarContainer;
    friend class HBarContainer;
    friend class RangeBarList;
protected:
    int row;
    int begin;
    int end;
public:
    explicit BarItem(QWidget* parent = nullptr);
    void syncDataToWidget() override;
    void syncWidgetToData() override;
signals:
    void sigUpdateWidget();
protected:
    void connectModelView() override;
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
    QVector<BarItem*> rangeWidgets;
public:
    inline int getMinVal() const;
    inline int getMaxVal() const;
    inline double getVpp() const;
    inline double getMaxVpp() const;
    inline double getMinVpp() const;
    inline bool isZoomEnabled() const;
    inline double getZoomStep() const;
protected:
    explicit AbstractBarContainer(QWidget* parent = nullptr);
    virtual void updateBarGeometry() = 0;                           //更新容器及其内部组件的大小
    virtual void updateRangeWidgetGeometry(BarItem* rw) = 0;   //更新RangeWidget的位置和大小
    void wheelEvent(QWheelEvent *event) override;
signals:
    void sigZoom();
};

class VBarContainer : public AbstractBarContainer {
    friend class RangeBar;
protected:
    explicit VBarContainer(QWidget* parent = nullptr);
    void updateBarGeometry() override;
    void updateRangeWidgetGeometry(BarItem *rw) override;
};

class HBarContainer : public AbstractBarContainer {
    friend class RangeBar;
protected:
    explicit HBarContainer(QWidget* parent = nullptr);
    void updateBarGeometry() override;
    void updateRangeWidgetGeometry(BarItem *rw) override;
};

class RangeBar : public ScrollArea {
    Q_OBJECT
public:
    enum BarUpdateType {
        Content,
        Zoom
    };
protected:
    bool assembled;
    bool isVertical;
    QWidget* rootContent;
    AbstractBarContainer* container;
public:
    explicit RangeBar(AbstractBarContainer* c, QWidget* parent = nullptr);
    inline void setData(ListData* d);
    inline void setBarRange(int minVal, int maxVal);
    inline void setZoomRange(double minVpp, double maxVpp);
    inline void setZoomEnabled(bool enabled = true);
    inline void setZoomStep(double step);
    inline void setVpp(double vpp);
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
    void initPeriodWidget(BarItem* rw);
    void barDataChanged(BarUpdateType type);
    void performHorizontalZoom();
    void performVerticalZoom();
};

inline int BarData::getBegin() const {
    return begin;
}

inline int BarData::getEnd() const {
    return end;
}

inline void BarData::setBegin(int b) {
    begin = b;
}

inline void BarData::setEnd(int e) {
    end = e;
}

inline int AbstractBarContainer::getMinVal() const {
    return minVal;
}

inline int AbstractBarContainer::getMaxVal() const {
    return maxVal;
}

inline double AbstractBarContainer::getVpp() const {
    return vpp;
}

inline double AbstractBarContainer::getMaxVpp() const {
    return maxVpp;
}

inline double AbstractBarContainer::getMinVpp() const {
    return minVpp;
}

inline bool AbstractBarContainer::isZoomEnabled() const {
    return zoomEnabled;
}

inline double AbstractBarContainer::getZoomStep() const {
    return zoomStep;
}

inline void RangeBar::setData(ListData *d) {
    ScrollArea::setData(d);
}

inline void RangeBar::setBarRange(int minVal, int maxVal) {
    if (minVal >= maxVal) {
        throwInFunc("invalid range");
    }
    container->minVal = minVal;
    container->maxVal = maxVal;
}

inline void RangeBar::setZoomRange(double minVpp, double maxVpp) {
    if (minVpp >= maxVpp) {
        throwInFunc("invalid range");
    }
    container->minVpp = minVpp;
    container->maxVpp = maxVpp;
}

inline void RangeBar::setZoomEnabled(bool enabled) {
    container->zoomEnabled = enabled;
}

inline void RangeBar::setZoomStep(double step) {
    if (step <= 0) {
        throwInFunc("invalid step");
    }
    container->zoomStep = step;
}

inline void RangeBar::setVpp(double vpp) {
    container->vpp = vpp;
}

#endif //LIFERHYTHM_RANGEBAR_H
