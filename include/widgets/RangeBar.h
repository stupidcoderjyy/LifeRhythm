//
// Created by stupid_coder_jyy on 2024/4/2.
//

#ifndef LIFERHYTHM_RANGEBAR_H
#define LIFERHYTHM_RANGEBAR_H

#include "Widget.h"
#include "ScrollArea.h"
#include "Error.h"
#include "NestedListData.h"

class CORE_API BarData : public NestedListNode {
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

class CORE_API BarItem : public Widget {
    Q_OBJECT
    friend class VBarContainer;
    friend class HBarContainer;
    friend class RangeBar;
protected:
    int row;
    int begin;
    int end;
public:
    explicit BarItem(QWidget* parent = nullptr, bool initInConstructor = true);
    void syncDataToWidget() override;
    void syncWidgetToData() override;
signals:
    void sigUpdateWidget();
protected:
    void connectModelView() override;
};

class CORE_API AbstractBarContainer : public QWidget {
    Q_OBJECT
    friend class RangeBar;
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
    explicit AbstractBarContainer(QWidget* parent = nullptr);
    ~AbstractBarContainer() override;
    virtual void updateBarGeometry() = 0;                     //更新容器及其内部组件的大小
    virtual void updateRangeWidgetGeometry(BarItem* rw) = 0;  //更新RangeWidget的位置和大小
    void wheelEvent(QWheelEvent *event) override;
signals:
    void sigMainZoom();
    void sigSideZoom();
};

class CORE_API VBarContainer : public AbstractBarContainer {
public:
    explicit VBarContainer(QWidget* parent = nullptr);
protected:
    void updateBarGeometry() override;
    void updateRangeWidgetGeometry(BarItem *rw) override;
};

class CORE_API HBarContainer : public AbstractBarContainer {
    Q_OBJECT
public:
    explicit HBarContainer(QWidget* parent = nullptr);
protected:
    void updateBarGeometry() override;
    void updateRangeWidgetGeometry(BarItem *rw) override;
    void wheelEvent(QWheelEvent *event) override;
signals:
    void sigScroll(int dx);
};

class CORE_API RangeBar : public ScrollArea {
    Q_OBJECT
public:
    enum BarUpdateType {
        Init,
        BarCountChanged,
        MainZoom,
        SideZoom
    };
protected:
    AbstractBarContainer* container;
    QWidget* rootContent;
    bool assembled;
    bool isVertical;
public:
    explicit RangeBar(AbstractBarContainer* container, QWidget* parent = nullptr);
    void syncDataToWidget() override;
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void setData(WidgetData* d) override;
    inline void setBarRange(int minVal, int maxVal) const;
    inline void setZoomEnabled(bool main, bool side) const;
    inline void setMainZoomRange(double minVpp, double maxVpp) const;
    inline void setMainZoomStep(double step) const;
    inline void setVpp(double vpp) const;
    inline void setSideZoomStep(int step) const;
    inline void setSideZoomRange(int minSideLen, int maxSideLen) const;
    inline void setSideLen(int len) const;
signals:
    void sigBarLayoutChanged();
protected:
    void connectModelView() override;
    void resizeEvent(QResizeEvent *event) override;
    virtual BarItem* createRangeWidget();
    virtual void assembleContainer();
    virtual void updateContentSize();
private:
    void initPeriodWidget(BarItem *rw);
    void barDataChanged(BarUpdateType type);
    void performHorizontalZoom();
    void performVerticalZoom();
    void assemble();
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

inline bool AbstractBarContainer::isMainZoomEnabled() const {
    return mainZoomEnabled;
}

inline double AbstractBarContainer::getMainZoomStep() const {
    return mainZoomStep;
}

inline bool AbstractBarContainer::isSideZoomEnabled() const {
    return sideZoomEnabled;
}

inline int AbstractBarContainer::getSideZoomStep() const {
    return sideZoomStep;
}

inline int AbstractBarContainer::getMinSideLen() const {
    return minSideLen;
}

inline int AbstractBarContainer::getMaxSideLen() const {
    return maxSideLen;
}

inline int AbstractBarContainer::getSideLen() const {
    return sideLen;
}

inline void RangeBar::setBarRange(const int minVal, const int maxVal) const {
    if (minVal >= maxVal) {
        throwInFunc("invalid bar range");
    }
    container->minVal = minVal;
    container->maxVal = maxVal;
}

inline void RangeBar::setZoomEnabled(const bool main, const bool side) const {
    container->mainZoomEnabled = main;
    container->sideZoomEnabled = side;
}

inline void RangeBar::setMainZoomRange(const double minVpp, const double maxVpp) const {
    if (minVpp >= maxVpp) {
        throwInFunc("invalid main zoom range");
    }
    container->minVpp = minVpp;
    container->maxVpp = maxVpp;
}

inline void RangeBar::setMainZoomStep(const double step) const {
    container->mainZoomStep = step;
}

inline void RangeBar::setVpp(const double vpp) const {
    container->vpp = vpp;
}

inline void RangeBar::setSideZoomStep(const int step) const {
    container->sideZoomStep = step;
}

inline void RangeBar::setSideZoomRange(const int minSideLen, const int maxSideLen) const {
    if (minSideLen >= maxSideLen) {
        throwInFunc("invalid side zoom range");
    }
    container->minSideLen = minSideLen;
    container->maxSideLen = maxSideLen;
}

inline void RangeBar::setSideLen(const int len) const {
    container->sideLen = len;
}

#endif //LIFERHYTHM_RANGEBAR_H