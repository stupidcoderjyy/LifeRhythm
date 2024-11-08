//
// Created by stupid_coder_jyy on 2024/2/10.
//

#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include "TabBar.h"
#include "ImgLabel.h"
#include "TextLabel.h"
#include "RcManagers.h"

USING_NAMESPACE(lr)

TabWidget::TabWidget(QWidget *parent) : Widget(parent) {
}

void TabWidget::onTabCreated() {
}

void TabWidget::onTabHidden() {
}

void TabWidget::onTabDestroyed() {
}

void TabWidget::onTabShown() {
}

TabCard::TabCard(QWidget *parent) : Widget(parent) {
}

void TabCard::onFinishedParsing(Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        auto* tab = static_cast<TabCard*>(target);
        tab->iconLabel = tab->getPointer<ImgLabel>("icon");
        tab->titleLabel = tab->getPointer<TextLabel>("title");
        tab->closeButton = tab->getPointer<CloseButton>("closeButton");
        connect(tab->closeButton, &CloseButton::sigActivated, tab, &TabCard::sigTabClosed);
    };
}

void TabCard::mousePressEvent(QMouseEvent *event) {
    emit sigTabActivated();
    selected = true;
    setState(3);
}

void TabCard::enterEvent(QEvent *event) {
    setState(selected ? 3 : 1);
}

void TabCard::leaveEvent(QEvent *event) {
    setState(selected ? 2 : 0);
}

void TabCard::setIcon(const Identifier &id) {
    QPixmap* img = ImageStorage::get(id);
    if (img) {
        if (img->width() == 30 && img->height() == 30) {
            iconLabel->setPixmap(*img);
        } else {
            iconLabel->setPixmap(img->scaled(30,30,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
    }
}

void TabCard::setTitle(const QString &title) {
    titleLabel->setText(title);
}

void TabCard::setActivated() {
    if (!selected) {
        selected = true;
        setState(state + 2);
    }
}

void TabCard::setHidden() {
    if (selected) {
        selected = false;
        setState(state - 2);
    }
}

CloseButton::CloseButton(QWidget *parent) : ImgLabel(parent) {
}

void CloseButton::enterEvent(QEvent *event) {
    setState(1);
}

void CloseButton::leaveEvent(QEvent *event) {
    setState(0);
}

void CloseButton::mousePressEvent(QMouseEvent *ev) {
    ev->accept();   //阻止事件传递到T父控件(TabBar)，使得标签页被选中
}

TabBar::TabBar(QWidget *parent) : QScrollArea(parent) {
}

void TabBar::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<TabBar*>(target)->init();
    };
}

TabBar::~TabBar() {
    for(auto& t : tabs) {
        delete t;
    }
}

void TabBar::selectTab(Tab *tab) {
    if (selected == tab) {
        return;
    }
    if (selected) {
        selected->card->setHidden();
        selected->content->onTabHidden();
    }
    tab->card->setActivated();
    tab->content->onTabShown();
    emit sigTabContentChanged(selected, tab);
    selected = tab;
}

void TabBar::insertTab(const QString &title, TabWidget *content, const Identifier &icon) {
    static WidgetFactory* tabLoader = WidgetFactoryStorage::get("lr:widget_tabcard");
    auto* tabCard = static_cast<TabCard*>(tabLoader->apply(this));
    tabCard->setTitle(title);
    tabCard->setIcon(icon);
    auto* tab = new Tab(content, tabCard);
    if (tabs.empty()) {
        setFixedHeight(61);
    }
    tabs << tab;
    connect(tabCard, &TabCard::sigTabClosed, this, [this, tab](){
        closeTab(tab);
    });
    connect(tabCard, &TabCard::sigTabActivated, this, [this, tab]{
        selectTab(tab);
    });
    content->onTabCreated();
    content->setParent(this);
    selectTab(tab);
    hLayout->addWidget(tabCard);
    checkBarPos = true;
}

void TabBar::closeTab(Tab *tab) {
    tab->content->onTabDestroyed();
    int oldPos = tabs.indexOf(tab);
    int oldSize = tabs.size();
    tabs.remove(oldPos);
    if (selected && selected == tab && oldSize > 1) {
        if (oldPos == 0) {
            selectTab(tabs[0]); //打开后一个
        } else {
            selectTab(tabs[oldPos - 1]); //打开前一个
        }
    }
    delete tab;
    if (tabs.empty()) {
        setFixedHeight(0);
        selected = nullptr;
    }
}

bool TabBar::event(QEvent *event) {
    if (event->type() == QEvent::Wheel) {
        auto* we = static_cast<QWheelEvent*>(event);
        QScrollBar* bar = horizontalScrollBar();
        bar->setValue(bar->value() - we->angleDelta().y());
    }
    return QScrollArea::event(event);
}

void TabBar::closeAll() {
    for (auto* t : tabs) {
        t->content->onTabDestroyed();
        delete t;
    }
    tabs.clear();
    selected = nullptr;
    setFixedHeight(0);
}

void TabBar::init() {
    setFrameShape(QFrame::NoFrame);
    setWidgetResizable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAttribute(Qt::WA_MouseNoMask);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    setFixedHeight(61);
    viewport()->setObjectName("vp");
    viewport()->setStyleSheet(qss_object("vp", bg(Styles::GRAY_0->rgbHex)));
    verticalScrollBar()->setVisible(false);
    contents = new ContentWidget(this);
    contents->setObjectName("contents");
    contents->setStyleSheet(qss_object("contents", bg(Styles::GRAY_0->rgbHex)));
    contents->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    hLayout = new QHBoxLayout(contents);
    hLayout->setContentsMargins(0,0,0,1);
    hLayout->setAlignment(Qt::AlignLeft);
    hLayout->setSpacing(0);
    contents->setLayout(hLayout);
    setWidget(contents);
    connect(contents, &ContentWidget::sigSizeUpdated, this, [this](){
        if (checkBarPos) {
            if (width() < contents->width()) {
                auto* bar = horizontalScrollBar();
                bar->setValue(bar->maximum());
            }
            checkBarPos = false;
        }
    });
}

Tab::Tab(TabWidget *content, TabCard *card) : content(content), card(card) {}

Tab::~Tab() {
    delete content;
    delete card;
}

ContentWidget::ContentWidget(QWidget *parent):QWidget(parent) {
}

void ContentWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    emit sigSizeUpdated();
}
