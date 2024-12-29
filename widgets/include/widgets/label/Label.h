//
// Created by stupid_coder_jyy on 2024/2/8.
//

#ifndef LABEL_H
#define LABEL_H

#include <QLabel>
#include "Identifier.h"
#include "StandardWidget.h"
#include "Styles.h"

BEGIN_LR

class Label : public QLabel, public StandardWidget {
public:
    explicit Label(QWidget* parent, bool initInConstructor = true);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    void onStateRespondersParsing(Handlers &responders, NBT *stateTag) override;
    inline void setSizeToText();
    static QSize baseTextSize(const QString& text, const QFont& font);
protected:
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
};

inline void Label::setSizeToText() {
    setFixedSize(baseTextSize(text(), font()));
}

END_LR

#endif //LABEL_H
