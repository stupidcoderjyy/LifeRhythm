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

USING_NAMESPACE(lr)

MainFrame::MainFrame(QWidget* parent): Widget(parent) {
}

void MainFrame::onFinishedParsing(StandardWidget::Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget* target) {
        static_cast<MainFrame*>(target)->initWidget();
    };
}

void MainFrame::initWidget() {
    tabBar = getPointer<TabBar>("tabBar");
    auto* tabContent = getPointer<Widget>("tabContent");
    connect(tabBar, &TabBar::sigTabContentChanged, this, [this](Tab* pre, Tab* cur){
        switchSingleLayoutWidget(layoutTabContent, pre ? pre->content : nullptr, cur->content);
    });
    layoutTabContent = tabContent->layout();
    auto* lr = LifeRhythm::get();
    if (lr->getConfig().getMode() == Config::Test) {
        setVisible(false);
    } else {
        showFullScreen();
    }
}
