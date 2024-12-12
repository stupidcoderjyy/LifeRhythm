//
// Created by stupid_coder_jyy on 2024/1/14.
//

#ifndef LIFERHYTHM_STANDARDWIDGET_H
#define LIFERHYTHM_STANDARDWIDGET_H

#include <QMap>
#include <QWidget>
#include "WidgetData.h"
#include "Plugin.h"

class NBT;

class CORE_API StandardWidget {
    friend class WidgetFactory;
public:
    typedef std::function<void(QWidget*)> Handler;
    typedef QVector<Handler> Handlers;
protected:
    QMap<QString, QWidget*> pointers{};
    QMap<int, Handlers*> stateResponders{};
    Handlers globalResponders{};
    int state = -1;
    WidgetData* wData = nullptr;
    QVector<QMetaObject::Connection> dc;

    /*
     *  标志组件是否初始化完成，由用户控制
     *  在prepared为false时调用setData应当报错
     */
    bool prepared;
public:
    explicit StandardWidget(bool initInConstructor);
    void registerResponder(int _state, const Handler& responder);
    void registerGlobalResponder(const Handler& responder);
    void setState(int newState);

    /*
     * 完成类型的解析和标准化组件对象创建后被调用
     */
    virtual void onPreParsing(Handlers &handlers, NBT* nbt);

    /*
     * 完成本组件解析，但还没有开始解析子组件时被调用
     */
    virtual void onPostParsing(Handlers &handlers, NBT* nbt);

    /*
     * 在完成本组件和所有子组件解析时被调用
     * 加入handlers的逻辑先于initWidget被调用
     */
    virtual void onFinishedParsing(Handlers &handlers, NBT* nbt);
    //自定义条件触发器的解析逻辑
    virtual void onStateRespondersParsing(Handlers &responders, NBT* nbt);
    void registerPointer(const QString& id, QWidget* p);
    void registerPointer(const StandardWidget* other);
    template<class T> T* getPointer(const QString& id) {
        auto* result = pointers.value(id, nullptr);
        return dynamic_cast<T*>(result);
    }

    /*
     *  设置组件和子组件的WidgetData
     *  如果需要更新组件则需要自行调用syncDataToWidget
     *  如果调用时prepared为false，则会先进行初始化
     */
    virtual void setData(WidgetData* d);

    /*
     * 将data中数据同步到组件中，data可能为null
     */
    virtual void syncDataToWidget();

    /*
     * 将组件内的数据同步到data中，data可能为null
     */
    virtual void syncWidgetToData();

    virtual ~StandardWidget();
    inline WidgetData* widgetData() const;
    int getState() const;
protected:
    /*
     *  连接wData和组件之间的信号，会在设置新的WidgetData时被调用，
     *  创建的信号应当加入dc列表，dc列表中存储的信号会在设置新的
     *  WidgetData时清楚。
     *  !! 和wData无关的信号别放在这里连接，应当考虑initWidget
     */
    virtual void connectModelView();

    /*
     * 初始化组件。在使用WidgetFactory构造时，本函数会在最后被调用；
     * 使用new构造时，若传入initInConstructor为true，则本函数会在
     * Qt下一轮事件循环中被调用，传入false时不会被自动调用
     * !! 此函数不应当做任何与WidgetData有关的事情
     */
    virtual void initWidget();
};

inline WidgetData * StandardWidget::widgetData() const {
    return wData;
}

inline int StandardWidget::getState() const {
    return state;
}
#endif //LIFERHYTHM_STANDARDWIDGET_H
