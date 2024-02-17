//
// Created by stupid_coder_jyy on 2024/2/10.
//

#ifndef LIFERHYTHM_TABBAR_H
#define LIFERHYTHM_TABBAR_H

#include "widgets/Widget.h"
#include "widgets/ImgLabel.h"
#include "Identifier.h"
#include "Namespaces.h"
#include <QScrollArea>
#include <QHBoxLayout>

class WidgetFactory;
class TextLabel;

LR_BEGIN
class CloseButton;

class TabWidget : public Widget {
    friend class TabBar;
public:
    explicit TabWidget(QWidget* parent = nullptr);
protected:
    virtual void onTabOpen();
    virtual void onTabHidden();
    virtual void onTabClosed();
    virtual void onTabActivated();
};

class TabCard : public Widget{
    Q_OBJECT
private:
    bool selected = false;
    ImgLabel* iconLabel{};
    TextLabel* titleLabel{};
    CloseButton* closeButton{};
public:
    explicit TabCard(QWidget* parent);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void setIcon(const Identifier& id);
    void setTitle(const QString& title);
    void setActivated();
    void setHidden();
signals:
    void sigTabClosed();
    void sigTabActivated();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

class CloseButton : public ImgLabel {
Q_OBJECT
public:
    explicit CloseButton(QWidget* parent);
protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;
};

class Tab{
public:
    TabWidget* content;
    TabCard* card;
    Tab(TabWidget *content, TabCard *card);
    ~Tab();
};

class ContentWidget : public QWidget {
    Q_OBJECT
public:
    explicit ContentWidget(QWidget* parent = nullptr);
signals:
    void sigSizeUpdated();
protected:
    void resizeEvent(QResizeEvent *event) override;
PAINT_QSS
};

class TabBar : public QScrollArea{
    Q_OBJECT
private:
    QVector<Tab*> tabs{};
    Tab* selected{};
    ContentWidget* contents{};
    QHBoxLayout* hLayout;
    bool checkBarPos{};
    static WidgetFactory* tabLoader;
public:
    static void mainInit();
    explicit TabBar(QWidget* parent);
    void selectTab(Tab* tab);
    void insertTab(const QString& title, TabWidget* tab, const Identifier& icon = "lr:icon_30");
    void closeTab(Tab* tab);
    void closeAll();
    ~TabBar() override;
signals:
    void sigTabContentChanged(QWidget* content);
protected:
    bool event(QEvent *event) override;
};

LR_END

#endif //LIFERHYTHM_TABBAR_H
