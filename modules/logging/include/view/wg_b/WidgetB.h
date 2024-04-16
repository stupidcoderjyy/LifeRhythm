//
// Created by stupid_coder_jyy on 2024/4/15.
//

#ifndef LIFERHYTHM_WIDGETB_H
#define LIFERHYTHM_WIDGETB_H

#include "TabBar.h"
#include "Namespaces.h"
#include "WeekView.h"
#include "WeekViewTitle.h"

BEGIN_NAMESPACE(lr::log)

class WidgetB : public TabWidget {
    Q_OBJECT
private:
    WeekViewTitle* wvTitle;
    WeekView* wv;
public:
    explicit WidgetB(QWidget* parent = nullptr);
protected:
    void onTabCreated() override;
};

END_NAMESPACE

#endif //LIFERHYTHM_WIDGETB_H
