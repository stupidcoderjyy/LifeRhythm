//
// Created by stupid_coder_jyy on 2024/2/11.
//

#include "MainFrame.h"
#include "Constants.h"
#include "Styles.h"
#include "TabBar.h"
#include <QVBoxLayout>

USING_LR

MainFrame::MainFrame():QWidget() {
    setObjectName(Constants::NAME);
    showFullScreen();
    setStyleSheet(qss_t(Constants::NAME, bg(Styles::BLACK)));
    auto* vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    vLayout->setAlignment(Qt::AlignTop);
    tabBar = new TabBar(this);
    vLayout->addWidget(tabBar);
    setLayout(vLayout);
}
