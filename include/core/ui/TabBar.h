//
// Created by stupid_coder_jyy on 2024/2/10.
//

#ifndef LIFERHYTHM_TABBAR_H
#define LIFERHYTHM_TABBAR_H

#include "widgets/Widget.h"
#include "Identifier.h"
#include "Namespaces.h"
#include <QScrollArea>
#include <QHBoxLayout>
#include "Plugin.h"
#include <QPainter>

#include "Button.h"
#include "Label.h"

class WidgetFactory;

BEGIN_NAMESPACE(lr)

class CloseButton;

class CORE_API TabWidget : public Widget {
    friend class TabBar;
public:
    explicit TabWidget(QWidget* parent = nullptr);
protected:
    virtual void onTabCreated();
    virtual void onTabHidden();
    virtual void onTabDestroyed();
    virtual void onTabShown();
};

class CORE_API TabCard : public Widget{
    Q_OBJECT
private:
    bool selected = false;
    Label* iconLabel{};
    Label* titleLabel{};
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

class CORE_API CloseButton : public Button {
Q_OBJECT
public:
    explicit CloseButton(QWidget* parent);
protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;
};

class CORE_API Tab{
public:
    TabWidget* content;
    TabCard* card;
    Tab(TabWidget *content, TabCard *card);
    ~Tab();
};

class CORE_API ContentWidget : public QWidget {
    Q_OBJECT
public:
    explicit ContentWidget(QWidget* parent = nullptr);
signals:
    void sigSizeUpdated();
protected:
    void resizeEvent(QResizeEvent *event) override;
PAINT_QSS
};

class CORE_API TabBar : public QScrollArea, public StandardWidget{
    friend class MainFrame;
    Q_OBJECT
private:
    QVector<Tab*> tabs{};
    Tab* selected{};
    ContentWidget* contents{};
    QHBoxLayout* hLayout;
    bool checkBarPos{};
public:
    static void mainInit();
    explicit TabBar(QWidget* parent);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void selectTab(Tab* tab);
    void insertTab(const QString& title, TabWidget* tab, const Identifier& icon = Identifier("lr:icon_30"));
    void closeTab(Tab* tab);
    void closeAll();
    ~TabBar() override;
signals:
    void sigTabContentChanged(Tab* prev, Tab* cur);
protected:
    bool event(QEvent *event) override;
private:
    void initWidget();
};

END_NAMESPACE

#endif //LIFERHYTHM_TABBAR_H
