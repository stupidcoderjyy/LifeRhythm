//
// Created by stupid_coder_jyy on 2024/3/13.
//

#ifndef FOCUSMANAGER_H
#define FOCUSMANAGER_H

#include <QWidget>

#include "Namespaces.h"
#include "Plugin.h"

BEGIN_LR

class CORE_API FocusManager {
private:
    static FocusManager* manager;
    QWidget* focusedWidget;
    QVector<QWidget*> focusStack;
public:
    static void mark(QWidget* w);       //清除上个组件的焦点并设置传入组件的焦点
    static void push(QWidget* w);       //将当前组件入栈，不执行任何操作
    static void pop();                  //设置栈顶组件焦点
    static bool checkPeek(QWidget* w);  //判断栈顶组件是否为传入的组件
    static void popIfPeekMatch(QWidget* w);
    static bool checkCurrent(QWidget* w);
protected:
    FocusManager();
    void mark0(QWidget* w);
    void push0(QWidget* w);
    void pop0();
    bool checkPeek0(const QWidget* w);
};

END_LR

#endif //FOCUSMANAGER_H
