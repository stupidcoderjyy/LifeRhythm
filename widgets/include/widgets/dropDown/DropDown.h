//
// Created by JYY on 24-11-24.
//

#ifndef DROPDOWN_H
#define DROPDOWN_H

#include "FocusContainer.h"
#include "LineEdit.h"
#include "Menu.h"

BEGIN_LR

class DropDownMenu : public Menu {
    Q_OBJECT
public:
    explicit DropDownMenu(QWidget *parent = nullptr);
signals:
    void sigSelectOption();
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
};

class DropDown : public FocusContainer {
    Q_OBJECT
public:
    enum Style {
        Jetbrains,
        Button
    };
protected:
    DropDownMenu *menu;
    bool pressLock;
    bool menuOpen;
    Style style;
public:
    explicit DropDown(QWidget *parent = nullptr, bool initInConstructor = true);
    void onPostParsing(Handlers &handlers, NBT *nbt) override;
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    ~DropDown() override;
    inline DropDownMenu *getMenu() const;
    inline void setStyle(Style style);
signals:
    void sigMenuOpening();
    void sigMenuClosing();
    void sigCloseMenu();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    QString getNormalQss() override;
    QString getFocusedQss() override;
    void initWidget() override;
private:
    void init0();
};

inline DropDownMenu * DropDown::getMenu() const {
    return menu;
}

inline void DropDown::setStyle(Style s) {
    style = s;
}

END_LR

#endif //DROPDOWN_H
