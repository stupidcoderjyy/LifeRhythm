//
// Created by stupid_coder_jyy on 2024/2/9.
//

#ifndef LIFERHYTHM_TABWIDGET_H
#define LIFERHYTHM_TABWIDGET_H

#include "StdWidget.h"
#include "StdImgLabel.h"
#include "Identifier.h"

class StdTextLabel;
class CloseButton;

class TabWidget : public StdWidget{
    Q_OBJECT
private:
    bool selected = false;
    StdImgLabel* iconLabel{};
    StdTextLabel* titleLabel{};
    CloseButton* closeButton{};
public:
    explicit TabWidget(QWidget* parent);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void setIcon(const Identifier& id);
    void setTitle(const QString& title);
signals:
    void sigTabClosed();
    void sigTabActivated();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

class CloseButton : public StdImgLabel {
    Q_OBJECT
public:
    explicit CloseButton(QWidget* parent);
protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
signals:
    void sigActivated();
};


#endif //LIFERHYTHM_TABWIDGET_H
