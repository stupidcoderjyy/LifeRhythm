//
// Created by stupid_coder_jyy on 2024/1/14.
//

#ifndef LIFERHYTHM_STANDARDWIDGET_H
#define LIFERHYTHM_STANDARDWIDGET_H

#include <QMap>
#include <QWidget>

class NBT;

class StandardWidget {
public:
    typedef std::function<void(QWidget*)> Handler;
    typedef QVector<Handler> Handlers;
protected:
    QMap<QString, QWidget*> pointers{};
    QMap<int, Handlers*> stateResponders{};
    Handlers globalResponders{};
    int state = -1;

public:
    int getState() const;
    void registerResponder(int _state, const Handler& responder);
    void registerGlobalResponder(const Handler& responder);
    void setState(int newState);
    /**
     * WidgetFactory在完成类型的解析和标准化组件对象创建后会立刻调用此函数
     * @param handlers WidgetFactory中的工作流
     * @param widgetTag 待解析的组件标签
     */
    virtual void onPreParsing(Handlers &handlers, NBT* widgetTag);
    /**
     * WidgetFactory在完成本组件解析，但还没有开始解析子组件时会调用此函数
     * @param handlers WidgetFactory中的工作流
     * @param widgetTag 待解析的组件标签
     */
    virtual void onPostParsing(Handlers &handlers, NBT* widgetTag);
    /**
     * WidgetFactory在完成本组件和所有子组件解析时会调用此函数
     * @param handlers WidgetFactory中的工作流
     * @param widgetTag 待解析的组件标签
     */
    virtual void onFinishedParsing(Handlers &handlers, NBT* widgetTag);
    /**
     * 自定义条件触发器的解析逻辑
     * @param responders 触发器集合，向该集合中添加触发器以实现注册
     * @param stateTag 状态标签
     */
    virtual void onStateRespondersParsing(Handlers &responders, NBT* stateTag);
    void registerPointer(const QString& id, QWidget* p);
    template<class T> T* getPointer(const QString& id) {
        auto* result = pointers.value(id, nullptr);
        return dynamic_cast<T*>(result);
    }
    virtual ~StandardWidget();
};

#endif //LIFERHYTHM_STANDARDWIDGET_H
