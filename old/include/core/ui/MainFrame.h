//
// Created by stupid_coder_jyy on 2024/2/11.
//

#ifndef LIFERHYTHM_MAINFRAME_H
#define LIFERHYTHM_MAINFRAME_H

#include "Namespaces.h"
#include "Widget.h"
#include "TabBar.h"
#include "Plugin.h"
#include <QLayout>

BEGIN_NAMESPACE(lr)

class TabBar;

class CORE_API MainFrame : public Widget {
    friend class LifeRhythm;
private:
    TabBar* tabBar;
    QLayout* layoutTabContent;
public:
    explicit MainFrame(QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
private:
    void initWidget();
};

END_NAMESPACE


#endif //LIFERHYTHM_MAINFRAME_H
