//
// Created by stupid_coder_jyy on 2024/2/8.
//

#ifndef LR_LABEL_H
#define LR_LABEL_H

#include <QLabel>
#include "StandardWidget.h"
#include "Styles.h"

class Label : public QLabel, public StandardWidget {
public:
    explicit Label(QWidget* parent, bool initInConstructor = true);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void onStateRespondersParsing(Handlers &responders, NBT *stateTag) override;
protected:
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
};

#endif //LABEL_H
