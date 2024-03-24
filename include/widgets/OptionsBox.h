//
// Created by stupid_coder_jyy on 2024/2/27.
//

#ifndef LIFERHYTHM_OPTIONSBOX_H
#define LIFERHYTHM_OPTIONSBOX_H

#include "FocusContainer.h"
#include "LineEdit.h"
#include "ArrowButton.h"
#include "WidgetData.h"
#include "Menu.h"

class OptionsMenu : public Menu {
    Q_OBJECT
public:
    explicit OptionsMenu(QWidget* parent = nullptr);
signals:
    void sigSelectOption();
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};

class DisplayOptionsButton : public ArrowButton {
public:
    explicit DisplayOptionsButton(QWidget* parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *evt) override;
};

class OptionsBoxLineEdit : public QLineEdit {
    friend class OptionsBox;
public:
    explicit OptionsBoxLineEdit(QWidget* parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
};

/*
    由输入框和一个按钮组成，点击按钮后，会在框下生成一个弹窗。通过这个弹窗
    可以获得想要的数据
*/
class OptionsBox : public FocusContainer {
protected:
    QLineEdit* optionEditor;
private:
    ArrowButton* buttonOpenMenu;
    OptionsMenu* menu;
    bool pressLock;
public:
    explicit OptionsBox(QWidget* parent = nullptr);
    ~OptionsBox() override;
protected:
    //将Menu传递过来的数据填入选择框中
    virtual void initMenu(OptionsMenu* menu);
    void resizeEvent(QResizeEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
private:
    void handleButtonClick(bool pressed);
    void onMenuClose();
};


#endif //LIFERHYTHM_OPTIONSBOX_H
