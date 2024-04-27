//
// Created by stupid_coder_jyy on 2024/4/27.
//

#ifndef LIFERHYTHM_USAGETYPELABEL_H
#define LIFERHYTHM_USAGETYPELABEL_H

#include "Namespaces.h"
#include "Widget.h"

BEGIN_NAMESPACE(lr::log)

class UsageTypeLabel : public Widget {
private:
    bool shouldDraw;
    QColor bgColor;
    QString name;
public:
    explicit UsageTypeLabel(QWidget* parent = nullptr);
    void syncDataToWidget() override;
protected:
    void connectModelView() override;
protected:
    void paintEvent(QPaintEvent *event) override;
};

END_NAMESPACE

#endif //LIFERHYTHM_USAGETYPELABEL_H
