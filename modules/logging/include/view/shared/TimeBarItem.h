//
// Created by stupid_coder_jyy on 2024/4/15.
//

#ifndef LIFERHYTHM_TIMEBARITEM_H
#define LIFERHYTHM_TIMEBARITEM_H

#include "RangeBar.h"
#include "TextLabel.h"
#include "Namespaces.h"

BEGIN_NAMESPACE(lr::log)

class TimeBarItem : public BarItem {
    friend class TimeBarContainer;
private:
    QColor colorBg;
    TextLabel* labelRange;
    TextLabel* labelInfo;
public:
    explicit TimeBarItem(QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    void syncDataToWidget() override;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void init();
};

END_NAMESPACE

#endif //LIFERHYTHM_TIMEBARITEM_H
