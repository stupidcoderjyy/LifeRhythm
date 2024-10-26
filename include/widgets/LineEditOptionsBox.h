//
// Created by stupid_coder_jyy on 2024/2/27.
//

#ifndef LIFERHYTHM_LINEEDITOPTIONSBOX_H
#define LIFERHYTHM_LINEEDITOPTIONSBOX_H

#include "AbstractOptionsBox.h"
#include "ArrowButton.h"

class CORE_API DisplayOptionsButton : public ArrowButton {
public:
    explicit DisplayOptionsButton(QWidget* parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *evt) override;
};

class CORE_API OptionsBoxLineEdit : public QLineEdit {
    friend class LineEditOptionsBox;
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
class CORE_API LineEditOptionsBox : public AbstractOptionsBox {
private:
    QLineEdit* optionEditor;
    ArrowButton* buttonOpenMenu;
public:
    explicit LineEditOptionsBox(QWidget* parent = nullptr);
protected:
    void onMenuClose() override;
    void clickBox(bool pressed) override;
};

#endif //LIFERHYTHM_LINEEDITOPTIONSBOX_H
