//
// Created by stupid_coder_jyy on 2024/2/11.
//

#ifndef LIFERHYTHM_MAINFRAME_H
#define LIFERHYTHM_MAINFRAME_H

#include "Namespaces.h"
#include "Widget.h"
#include "TabBar.h"
#include <QLayout>

LR_BEGIN

class TabBar;

class MainFrame : public Widget {
    friend class LifeRhythm;
private:
    TabBar* tabBar;
    QLayout* layoutTabContent;
public:
    static void mainInit();
    explicit MainFrame(QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
private:
    void init();
};

LR_END


#endif //LIFERHYTHM_MAINFRAME_H
