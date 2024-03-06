//
// Created by stupid_coder_jyy on 2024/2/11.
//

#include "MainFrame.h"
#include "Config.h"
#include "Styles.h"
#include "TabBar.h"
#include "LifeRhythm.h"
#include <QVBoxLayout>
#include "RcManagers.h"
#include "WidgetUtil.h"

USING_LR

void MainFrame::mainInit() {
    auto* mf = WidgetFactoryStorage::get("lr:mainframe");
    regClazz(mf, MainFrame);
    regClazz(mf, TabBar);
}

MainFrame::MainFrame(QWidget* parent): Widget(parent) {
}

void MainFrame::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<MainFrame*>(target)->init();
    };
}

void MainFrame::init() {
    tabBar = getPointer<TabBar>("tabBar");
    auto* tabContent = getPointer<Widget>("tabContent");
    connect(tabBar, &TabBar::sigTabContentChanged, this, [this](Tab* pre, Tab* cur){
        switchSingleLayoutWidget(layoutTabContent, pre ? pre->content : nullptr, cur->content);
    });
    layoutTabContent = tabContent->layout();
    showFullScreen();
}
