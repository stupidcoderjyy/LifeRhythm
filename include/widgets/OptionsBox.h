//
// Created by stupid_coder_jyy on 2024/3/25.
//

#ifndef LIFERHYTHM_ABSTRACTOPTIONSBOX_H
#define LIFERHYTHM_ABSTRACTOPTIONSBOX_H

#include "FocusContainer.h"
#include "LineEdit.h"
#include "Menu.h"

class CORE_API OptionsMenu : public Menu {
    Q_OBJECT
public:
    explicit OptionsMenu(QWidget* parent = nullptr);
signals:
    void sigSelectOption();
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
};

class CORE_API OptionsBox : public FocusContainer {
protected:
    OptionsMenu* menu;
    bool pressLock;
    bool menuOpen;
public:
    explicit OptionsBox(QWidget* parent = nullptr);
    ~OptionsBox() override;
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    virtual void clickBox(bool open);                    //点击生成弹窗或关闭弹窗
    virtual void initMenu(OptionsMenu* menu);           //初始化菜单组件
    virtual void onMenuClose();
};


#endif //LIFERHYTHM_ABSTRACTOPTIONSBOX_H
