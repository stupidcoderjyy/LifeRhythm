//
// Created by stupid_coder_jyy on 2024/3/20.
//

#ifndef LIFERHYTHM_USAGETYPEICON_H
#define LIFERHYTHM_USAGETYPEICON_H

#include "Widget.h"
#include "Namespaces.h"

BEGIN_NAMESPACE(lr::log)

class UsageTypeIcon : public Widget {
    friend class UsageTypeTreeItem;
public:
    explicit UsageTypeIcon(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

END_NAMESPACE

#endif //LIFERHYTHM_USAGETYPEICON_H
