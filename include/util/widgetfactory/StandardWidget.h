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
    bool prepared;
public:
    explicit StandardWidget(bool initInConstructor);
    int getState() const;
    void registerResponder(int _state, const Handler& responder);
    void registerGlobalResponder(const Handler& responder);
    void setState(int newState);
    //完成类型的解析和标准化组件对象创建后被调用
    virtual void onPreParsing(Handlers &handlers, NBT* nbt);
    //完成本组件解析，但还没有开始解析子组件时被调用
    virtual void onPostParsing(Handlers &handlers, NBT* nbt);
    //在完成本组件和所有子组件解析时被调用
    virtual void onFinishedParsing(Handlers &handlers, NBT* nbt);
    //自定义条件触发器的解析逻辑
    virtual void onStateRespondersParsing(Handlers &responders, NBT* nbt);
    void registerPointer(const QString& id, QWidget* p);
    void registerPointer(const StandardWidget* other);
    template<class T> T* getPointer(const QString& id) {
        auto* result = pointers.value(id, nullptr);
        return dynamic_cast<T*>(result);
    }
    virtual void setData(WidgetData* d);
    //将data中数据同步到组件中，data可能为null
    virtual void syncDataToWidget();
    //将组件内的数据同步到data中
    virtual void syncWidgetToData();
    virtual ~StandardWidget();
    WidgetData* widgetData() const {
        return wData;
    }
protected:
    virtual void connectModelView();

    //初始化组件，若使用WidgetFactory构造，则默认位于最后调用；其他情况下可以选择构造函数内调用
    virtual void initWidget();
};

#endif //LIFERHYTHM_STANDARDWIDGET_H
